#include <services/VoucherOrderServiceImpl.h>
#include <trantor/utils/Utilities.h>
#include <constants/RedisConstants.h>
#include <utils/RegexUtils.h>
#include <utils/RandomUtils.h>

using namespace hmdp;
using namespace drogon::internal;
using namespace drogon::nosql;
using namespace Poco::Redis;

VoucherOrderServiceImpl::VoucherOrderServiceImpl()
{
}

VoucherOrderServiceImpl::~VoucherOrderServiceImpl()
{
}

long VoucherOrderServiceImpl::seckillVoucher(long voucherId, TbUser loginUser)
{
    LOG_DEBUG << "[seckillVoucher] begin";

    //获取当前登陆的用户Id
    long userId = loginUser.getValueOfId();

    LOG_INFO << "[seckillVoucher] voucherId: " << voucherId << ",userId: " << userId;

    // 1.查询优惠券
    TbSeckillVoucher voucher = seckillVoucherMapper.findOne(Criteria(TbSeckillVoucher::Cols::_voucher_id, CompareOperator::EQ, std::to_string(voucherId)));
    // 2.判断秒杀是否开始
    trantor::Date now = trantor::Date::now();
    if (voucher.getValueOfBeginTime() > now)
    {
        throw BusinessException(ErrorCode::PARAMS_ERROR(), "秒杀尚未开始");
    }
    // 3.判断秒杀是否已经结束
    if (voucher.getValueOfEndTime() < now)
    {
        throw BusinessException(ErrorCode::PARAMS_ERROR(), "秒杀已经结束");
    }
    // 4.判断库存是否充足
    if (voucher.getValueOfStock() < 1)
    {
        throw BusinessException(ErrorCode::PARAMS_ERROR(), "库存不足");
    }

    LOG_DEBUG << "[seckillVoucher] end";

    return createVoucherOrder(voucherId, loginUser);
}

bool VoucherOrderServiceImpl::tryLock(const std::string &key, const std::string &val, const Poco::Timespan &expireTime)
{
    LOG_DEBUG << "[tryLock] begin";

    //创建锁，默认超时时间为10秒
    Command setCmd = Command::set(key, val, false, expireTime);
    std::string result = _redisClientPtr->execute<std::string>(setCmd);
    bool success = (result.compare("OK") == 0);

    LOG_INFO << "[tryLock] key:" << key << ",val:" << val << ",success:" << success;

    LOG_DEBUG << "[tryLock] end";
    return success;
}

void VoucherOrderServiceImpl::unLock(const std::string &key)
{
    LOG_DEBUG << "[unLock] begin";

    _redisClientPtr->execute<Poco::Int64>(Command::del(key));

    LOG_DEBUG << "[unLock] end";
};

long VoucherOrderServiceImpl::createVoucherOrder(long voucherId, TbUser loginUser)
{
    LOG_DEBUG << "[createVoucherOrder] begin";
    // 5.一人一单
    //获取当前登陆的用户Id
    long userId = loginUser.getValueOfId();

    // 创建锁对象
    std::string key = RedisConstants::LOCK_ORDER_KEY + std::to_string(userId);
    std::string val = "1";
    Poco::Timespan expireTime = Poco::Timespan(RedisConstants::LOCK_ORDER_TTL, 0);

    // 尝试获取锁
    bool isLock = tryLock(key, val, expireTime);
    // 判断
    if (!isLock)
    {
        // 获取锁失败，直接返回失败或者重试
        throw BusinessException(ErrorCode::PARAMS_ERROR(), "不允许重复下单");
    }

    try
    {
        // 5.1.查询订单
        auto orders = voucherOrderMapper.findBy(Criteria(TbVoucherOrder::Cols::_user_id, CompareOperator::EQ, userId) &&
                                                Criteria(TbVoucherOrder::Cols::_voucher_id, CompareOperator::EQ, voucherId));

        // 5.2.判断是否存在
        if (orders.size() > 0)
        {
            // 用户已经购买过了
            throw BusinessException(ErrorCode::PARAMS_ERROR(), "用户已经购买过一次");
        }

        LOG_DEBUG << "[createVoucherOrder] orders.size():" << orders.size();

        // 6.扣减库存
        Poco::Data::Session session = _sessionPoolPtr->get();
        std::string execSql = "update tb_seckill_voucher set stock=stock-1 where voucher_id=" + std::to_string(voucherId) + " and stock>0;";

        LOG_DEBUG << "[createVoucherOrder] execSql" << execSql;

        Poco::Data::Statement sql_update(session);
        sql_update << execSql;
        Poco::Data::RecordSet res(sql_update);
        int affectedRows = sql_update.execute();

        if (affectedRows == 0)
        {
            // 扣减失败
            throw BusinessException(ErrorCode::PARAMS_ERROR(), "库存不足");
        }

        // 7.创建订单
        TbVoucherOrder voucherOrder;
        // 7.1.订单id
        Poco::UUIDGenerator &generator = Poco::UUIDGenerator::defaultGenerator();
        Poco::UUID uuid(generator.create()); // time based
        LOG_DEBUG << "[createVoucherOrder] uuid:" << uuid.toString();
        //时间戳加4位随机数为orderId
        Poco::LocalDateTime now;
        std::string cur_time_str = Poco::DateTimeFormatter::format(now, "%Y%m%d%H%M%S");
        std::string final_id_str = cur_time_str + RandomUtils::getRandomNumberStr(4);
        LOG_DEBUG << "[createVoucherOrder] final_id_str:" << final_id_str;
        // TODO UUID TO LONG
        long orderId = atol(final_id_str.c_str()); // redisIdWorker.nextId("order");
        voucherOrder.setId(orderId);
        // 7.2.用户id
        voucherOrder.setUserId(userId);
        // 7.3.代金券id
        voucherOrder.setVoucherId(voucherId);
        // 7.4.插入数据
        voucherOrderMapper.insert(voucherOrder);

        // 释放锁
        unLock(key);

        LOG_DEBUG << "[createVoucherOrder] end";
        // 7.返回订单id
        return orderId;
    }
    catch (const Poco::Data::DataException &e)
    {
        LOG_ERROR << "DataException: " << e.message();
        throw BusinessException(ErrorCode::DB_ERROR(), "DataException:" + e.message());
    }
    catch (Poco::Redis::RedisException &e)
    {
        LOG_ERROR << "RedisException: " << e.message();
        throw BusinessException(ErrorCode::CACHE_ERROR(), "RedisException:" + e.message());
    }
}

/*
@Override public Result seckillVoucher(Long voucherId)
{
    Long userId = UserHolder.getUser().getId();
    // 1.执行lua脚本
    Long result = stringRedisTemplate.execute(
        SECKILL_SCRIPT,
        Collections.emptyList(),
        voucherId.toString(), userId.toString());
    int r = result.intValue();
    // 2.判断结果是否为0
    if (r != 0)
    {
        // 2.1.不为0 ，代表没有购买资格
        return Result.fail(r == 1 ? "库存不足" : "不能重复下单");
    }
    // 2.2.为0 ，有购买资格，把下单信息保存到阻塞队列
    VoucherOrder voucherOrder = new VoucherOrder();
    // 2.3.订单id
    long orderId = redisIdWorker.nextId("order");
    voucherOrder.setId(orderId);
    // 2.4.用户id
    voucherOrder.setUserId(userId);
    // 2.5.代金券id
    voucherOrder.setVoucherId(voucherId);
    // 2.6.放入阻塞队列
    orderTasks.add(voucherOrder);

    // 3.返回订单id
    return Result.ok(orderId);
}
@Override public Result seckillVoucher(Long voucherId)
{
    // 1.查询优惠券
    SeckillVoucher voucher = seckillVoucherService.getById(voucherId);
    // 2.判断秒杀是否开始
    if (voucher.getBeginTime().isAfter(LocalDateTime.now()))
    {
        // 尚未开始
        return Result.fail("秒杀尚未开始！");
    }
    // 3.判断秒杀是否已经结束
    if (voucher.getEndTime().isBefore(LocalDateTime.now()))
    {
        // 尚未开始
        return Result.fail("秒杀已经结束！");
    }
    // 4.判断库存是否充足
    if (voucher.getStock() < 1)
    {
        // 库存不足
        return Result.fail("库存不足！");
    }

    return createVoucherOrder(voucherId);
}

@Transactional public Result createVoucherOrder(Long voucherId)
{
    // 5.一人一单
    Long userId = UserHolder.getUser().getId();

    // 创建锁对象
    RLock redisLock = redissonClient.getLock("lock:order:" + userId);
    // 尝试获取锁
    boolean isLock = redisLock.tryLock();
    // 判断
    if (!isLock)
    {
        // 获取锁失败，直接返回失败或者重试
        return Result.fail("不允许重复下单！");
    }

    try
    {
        // 5.1.查询订单
        int count = query().eq("user_id", userId).eq("voucher_id", voucherId).count();
        // 5.2.判断是否存在
        if (count > 0)
        {
            // 用户已经购买过了
            return Result.fail("用户已经购买过一次！");
        }

        // 6.扣减库存
        boolean success = seckillVoucherService.update()
                              .setSql("stock = stock - 1") // set stock = stock - 1
                              .eq("voucher_id", voucherId)
                              .gt("stock", 0) // where id = ? and stock > 0
                              .update();
        if (!success)
        {
            // 扣减失败
            return Result.fail("库存不足！");
        }

        // 7.创建订单
        VoucherOrder voucherOrder = new VoucherOrder();
        // 7.1.订单id
        long orderId = redisIdWorker.nextId("order");
        voucherOrder.setId(orderId);
        // 7.2.用户id
        voucherOrder.setUserId(userId);
        // 7.3.代金券id
        voucherOrder.setVoucherId(voucherId);
        save(voucherOrder);

        // 7.返回订单id
        return Result.ok(orderId);
    }
    finally
    {
        // 释放锁
        redisLock.unlock();
    }
}
@Transactional public Result createVoucherOrder(Long voucherId)
{
    // 5.一人一单
    Long userId = UserHolder.getUser().getId();

    // 创建锁对象
    SimpleRedisLock redisLock = new SimpleRedisLock("order:" + userId, stringRedisTemplate);
    // 尝试获取锁
    boolean isLock = redisLock.tryLock(1200);
    // 判断
    if (!isLock)
    {
        // 获取锁失败，直接返回失败或者重试
        return Result.fail("不允许重复下单！");
    }

    try
    {
        // 5.1.查询订单
        int count = query().eq("user_id", userId).eq("voucher_id", voucherId).count();
        // 5.2.判断是否存在
        if (count > 0)
        {
            // 用户已经购买过了
            return Result.fail("用户已经购买过一次！");
        }

        // 6.扣减库存
        boolean success = seckillVoucherService.update()
                              .setSql("stock = stock - 1") // set stock = stock - 1
                              .eq("voucher_id", voucherId)
                              .gt("stock", 0) // where id = ? and stock > 0
                              .update();
        if (!success)
        {
            // 扣减失败
            return Result.fail("库存不足！");
        }

        // 7.创建订单
        VoucherOrder voucherOrder = new VoucherOrder();
        // 7.1.订单id
        long orderId = redisIdWorker.nextId("order");
        voucherOrder.setId(orderId);
        // 7.2.用户id
        voucherOrder.setUserId(userId);
        // 7.3.代金券id
        voucherOrder.setVoucherId(voucherId);
        save(voucherOrder);

        // 7.返回订单id
        return Result.ok(orderId);
    }
    finally
    {
        // 释放锁
        redisLock.unlock();
    }
}

@Transactional public Result createVoucherOrder(Long voucherId)
{
    // 5.一人一单
    Long userId = UserHolder.getUser().getId();

    synchronized(userId.toString().intern())
    {
        // 5.1.查询订单
        int count = query().eq("user_id", userId).eq("voucher_id", voucherId).count();
        // 5.2.判断是否存在
        if (count > 0)
        {
            // 用户已经购买过了
            return Result.fail("用户已经购买过一次！");
        }

        // 6.扣减库存
        boolean success = seckillVoucherService.update()
                              .setSql("stock = stock - 1") // set stock = stock - 1
                              .eq("voucher_id", voucherId)
                              .gt("stock", 0) // where id = ? and stock > 0
                              .update();
        if (!success)
        {
            // 扣减失败
            return Result.fail("库存不足！");
        }

        // 7.创建订单
        VoucherOrder voucherOrder = new VoucherOrder();
        // 7.1.订单id
        long orderId = redisIdWorker.nextId("order");
        voucherOrder.setId(orderId);
        // 7.2.用户id
        voucherOrder.setUserId(userId);
        // 7.3.代金券id
        voucherOrder.setVoucherId(voucherId);
        save(voucherOrder);

        // 7.返回订单id
        return Result.ok(orderId);
    }
}
*/
