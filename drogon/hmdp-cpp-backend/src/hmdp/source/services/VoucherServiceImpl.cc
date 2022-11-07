#include <services/VoucherServiceImpl.h>
#include <exception>

using namespace hmdp;
using namespace drogon::internal;
using namespace drogon::nosql;
using namespace Poco::Redis;
using Poco::Redis::Array;
using Poco::Redis::BulkString;

VoucherServiceImpl::VoucherServiceImpl()
{
}

VoucherServiceImpl::~VoucherServiceImpl()
{
}

void VoucherServiceImpl::addSeckillVoucher(TbVoucher &voucher, Json::Value jsonObj)
{
    LOG_DEBUG << "[addSeckillVoucher] begin";

    // 保存优惠券
    voucherMapper.insert(voucher);

    // 保存秒杀券信息
    // Stock,BeginTime,EndTime非数据库表 TbVoucher 中字段，在POST请求中加上
    TbSeckillVoucher seckillVoucher;
    seckillVoucher.setVoucherId(voucher.getValueOfId());
    seckillVoucher.setStock(jsonObj["stock"].asInt64());
    seckillVoucher.setBeginTime(trantor::Date::fromDbStringLocal(jsonObj["begin_time"].asString()));
    seckillVoucher.setEndTime(trantor::Date::fromDbStringLocal(jsonObj["end_time"].asString()));

    seckillVoucherMapper.insert(seckillVoucher);

    // 保存秒杀库存到Redis中
    std::string key = RedisConstants::SECKILL_STOCK_KEY + std::to_string(voucher.getValueOfId());
    std::string val = std::to_string(seckillVoucher.getValueOfStock());

    LOG_DEBUG << "[addSeckillVoucher] key:" << key << ",val:" << val;

    //TODO  Bad cast exception 
    Command redisCmd = Command::set(key, val);
    _redisClientPtr->execute<std::string>(redisCmd);

    LOG_DEBUG << "[addSeckillVoucher] end";
}

void VoucherServiceImpl::addVoucher(TbVoucher &voucher, Json::Value jsonObj)
{
    LOG_DEBUG << "[addSeckillVoucher] begin";

    // 保存优惠券
    voucherMapper.insert(voucher);

    LOG_DEBUG << "[addSeckillVoucher] end";
}

std::vector<TbVoucher> VoucherServiceImpl::queryVoucherOfShop(long shopId)
{
    LOG_DEBUG << "[queryVoucherOfShop] begin";

    auto vouchers = voucherMapper.findBy(Criteria(TbVoucher::Cols::_shop_id, CompareOperator::EQ, std::to_string(shopId)));

    LOG_DEBUG << "[queryVoucherOfShop] end";

    return vouchers;
}
