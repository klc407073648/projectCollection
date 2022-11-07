#include <services/ShopServiceImpl.h>
#include <trantor/utils/Utilities.h>
#include <constants/RedisConstants.h>
#include <utils/RegexUtils.h>
#include <utils/RandomUtils.h>
#include <utils/CommonUtils.h>
#include <constants/SystemConstants.h>
#include <cmath>
#include "Poco/Redis/Array.h"

using namespace hmdp;
using namespace drogon::internal;
using namespace drogon::nosql;
using namespace Poco::Redis;

ShopServiceImpl::ShopServiceImpl()
{
}

ShopServiceImpl::~ShopServiceImpl()
{
}

TbShop ShopServiceImpl::queryById(long id, CacheType type)
{
    LOG_DEBUG << "[queryById] begin";

    LOG_INFO << "[queryById]  id: " << id << ",type:" << type;

    if (type == CacheType::DEFAULT)
    {
        return getById(id);
    }
    else if (type == CacheType::CHCHE_PENETRATE)
    {
        return queryWithPassThrough(id);
    }
    else if (type == CacheType::CHCHE_AVALANCHE)
    {
        // TODO
        /*缓存雪崩：指在同一时段大量的缓存key同时失效或者Redis服务宕机，导致大量请求到达数据库，带来巨大压力。
        解决方案：

        * 给不同的Key的TTL添加随机值
        * 利用Redis集群提高服务的可用性
        * 给缓存业务添加降级限流策略
        * 给业务添加多级缓存
        * */
    }
    else if (type == CacheType::CHCHE_BREAKDOWN)
    {
        return queryWithMutex(id); //互斥锁
        // return queryWithLogicalExpire(id); //逻辑过期
    }

    LOG_DEBUG << "[queryById] end";
}

Json::Value ShopServiceImpl::queryShopCache(const std::string &key, const std::string &field)
{
    LOG_DEBUG << "[queryShopCache] begin";

    LOG_INFO << "[queryShopCache] key:" << key << ",field:" << field;

    Json::Value value;
    Command redisCmd = Command::exists(key);

    if (_redisClientPtr->execute<Poco::Int64>(redisCmd) == 1)
    {
        redisCmd = Command::hget(key, field);
        BulkString result = _redisClientPtr->execute<BulkString>(redisCmd);

        Json::Reader reader;
        if (reader.parse(result.value(), value))
        {
            LOG_DEBUG << "[queryShopCache] value:" << value.toStyledString();
        }
    }

    LOG_DEBUG << "[queryShopCache] end";

    return value;
}

void ShopServiceImpl::writeShopCache(const std::string &key, const std::string &field, const TbShop &shop, long expireTime)
{
    LOG_DEBUG << "[writeShopCache] begin";

    LOG_INFO << "[writeShopCache] key:" << key << ",field:" << field << ",expireTime:" << expireTime;
    LOG_INFO << "[writeShopCache] shop:[" << shop.toJson().toStyledString() << "]";

    Json::Value jsonValue = shop.toJson();
    jsonValue[field] = jsonValue.toStyledString();

    std::map<std::string, std::string> shopMap;
    shopMap = JsonUtils::ConvertJsonToMap(jsonValue);
    Command redisCmd = Command::hmset(key, shopMap);
    _redisClientPtr->execute<std::string>(redisCmd);

    redisCmd = Command::expire(key, expireTime);
    _redisClientPtr->execute<Poco::Int64>(redisCmd);

    LOG_DEBUG << "[writeShopCache] end";
}

void ShopServiceImpl::writeNullCache(const std::string &key, long expireTime)
{
    LOG_DEBUG << "[writeNullCache] begin";

    LOG_INFO << "[writeNullCache] key:" << key << ",expireTime:" << expireTime;
    Command redisCmd = Command::set(key, "", true, Poco::Timespan(expireTime, 0));
    _redisClientPtr->execute<BulkString>(redisCmd);

    LOG_DEBUG << "[writeNullCache] end";
}

/**
 * @brief 缓存击穿方案
 *        先从redis缓存中查询商铺信息，如果存在不为空返回信息，存在为空返回错误信息
 *        如果redis缓存中不存在，则查询数据库，数据库不存在，写入空值，然后返回nullptr
 *        数据库中存在，此时刷新缓存中的内容
 *
 */
TbShop ShopServiceImpl::queryWithPassThrough(long id)
{
    LOG_DEBUG << "[queryWithPassThrough] begin";

    try
    {
        std::string key = RedisConstants::CACHE_SHOP_KEY + std::to_string(id);
        std::string field = RedisConstants::ORIGIN_DATA_KEY;

        // 1.redis根据key查询商铺缓存
        Json::Value value = queryShopCache(key, field);

        // 2.redis缓存存在，则直接返回
        if (!value.isNull())
        {
            if (value == "")
            {
                throw BusinessException(ErrorCode::PARAMS_ERROR(), "商铺ID不存在");
            }
            return TbShop(value);
        }

        // 3.缓存不存在，根据id查询数据库
        TbShop shop = queryDbById(id);

        // 4.数据库不存在，返回错误
        if (shop.getValueOfId() <= 0)
        {
            // 将空值写入redis
            writeNullCache(key, RedisConstants::CACHE_NULL_TTL);
            // 返回错误信息
            throw BusinessException(ErrorCode::PARAMS_ERROR(), "商铺ID不存在");
        }

        // 5.存在，写入redis
        writeShopCache(key, field, shop, RedisConstants::CACHE_SHOP_TTL);

        LOG_DEBUG << "[queryWithPassThrough] end";
        return shop;
    }
    catch (Poco::Redis::RedisException &e)
    {
        LOG_ERROR << "RedisException: " << e.message();
        throw BusinessException(ErrorCode::CACHE_ERROR(), "RedisException:" + e.message());
    }
}

TbShop ShopServiceImpl::queryWithMutex(long id)
{
    LOG_DEBUG << "[queryWithMutex] begin";

    try
    {
        std::string key = RedisConstants::CACHE_SHOP_KEY + std::to_string(id);
        std::string field = RedisConstants::ORIGIN_DATA_KEY;

        // 1.redis根据key查询商铺缓存
        Json::Value value = queryShopCache(key, field);

        // 2.redis缓存存在，则直接返回
        if (!value.isNull())
        {
            LOG_DEBUG << "queryWithMutex value: " << value.toStyledString();
            if (value == "")
            {
                throw BusinessException(ErrorCode::PARAMS_ERROR(), "商铺ID不存在");
            }
            return TbShop(value);
        }

        // 4.实现缓存重建
        // 4.1.获取互斥锁
        std::string lockKey = RedisConstants::LOCK_SHOP_KEY + std::to_string(id);

        bool isLock = tryLock(lockKey);
        // 4.2.判断是否获取成功
        if (!isLock)
        {
            // 4.3.获取锁失败，休眠并重试
            Poco::Thread::sleep(50);
            return queryWithMutex(id);
        }
        // 4.4.获取锁成功，根据id查询数据库
        TbShop shop = queryDbById(id);
        // 5.不存在，返回错误
        if (shop.getValueOfId() <= 0)
        {
            // 将空值写入redis
            writeNullCache(key, RedisConstants::CACHE_NULL_TTL);
            // 返回错误信息
            throw BusinessException(ErrorCode::PARAMS_ERROR(), "商铺ID不存在");
        }
        // 6.存在，写入redis
        writeShopCache(key, field, shop, RedisConstants::CACHE_SHOP_TTL);

        // 7.释放锁
        unlock(lockKey);
        // 8.返回

        LOG_DEBUG << "[queryWithMutex] end";
        return shop;
    }
    catch (Poco::Redis::RedisException &e)
    {
        LOG_ERROR << "RedisException: " << e.message();
        throw BusinessException(ErrorCode::CACHE_ERROR(), "RedisException:" + e.message());
    }
}

//逻辑过期理论上应该，预先设置一把内容
//每次缓存重建会加30秒的时间，这时候属于未过期，直接返回店铺信息
TbShop ShopServiceImpl::queryWithLogicalExpire(long id)
{
    LOG_DEBUG << "[queryWithLogicalExpire] begin";

    try
    {
        std::string key = RedisConstants::CACHE_SHOP_KEY + std::to_string(id);
        setCurrentId(id);
        buildLogicalExpireCache(key, id);

        // 1.redis根据key查询商铺缓存
        Json::Value originData = queryShopCache(key, RedisConstants::ORIGIN_DATA_KEY);
        Json::Value expireTime = queryShopCache(key, RedisConstants::REDIS_DATA_EXPIRETIME_KEY);

        // 2.判断是否存在
        if ((!originData.isNull() && originData == "") || (!expireTime.isNull() && expireTime == ""))
        {
            // 3.存在，且为空的话，直接返回
            throw BusinessException(ErrorCode::PARAMS_ERROR(), "商铺ID缓存不存在");
        }

        // 4.命中，需要先把json反序列化为对象
        RedisData<TbShop> redisData;
        redisData.fromJsonStr(originData, expireTime);
        TbShop shop = redisData.getData();
        LocalDateTime expire = redisData.getExpireTime();
        LocalDateTime now;
        // 5.判断是否过期
        if (expire > now)
        {
            LOG_DEBUG << "[queryWithLogicalExpire] expire > now";
            // 5.1.未过期，直接返回店铺信息
            return shop;
        }
        // 5.2.已过期，需要缓存重建
        // 6.缓存重建
        // 6.1.获取互斥锁
        std::string lockKey = RedisConstants::LOCK_SHOP_KEY + std::to_string(id);
        bool isLock = tryLock(lockKey);

        LOG_DEBUG << "[queryWithLogicalExpire] isLock" << isLock;
        // 6.2.判断是否获取锁成功
        if (isLock)
        {
            try
            {
                // RunnableAdapter只能传对象和方法，修改setWithLogicalExpire的入参为空，通过setCurrentId获取当前商铺Id,
                // setWithLogicalExpire里面使用getCurrentId
                Poco::RunnableAdapter<ShopServiceImpl> ra(*this, &ShopServiceImpl::setWithLogicalExpire);
                _threadPoolPtr->start(ra);
            }
            catch (...)
            {
                throw BusinessException(ErrorCode::SYSTEM_ERROR(), "逻辑过期独立线程执行异常");
            }

            // 释放锁
            unlock(lockKey);
        };
        LOG_DEBUG << "[queryWithLogicalExpire] end";
        // 6.4.返回过期的商铺信息
        return shop;
    }
    catch (Poco::Redis::RedisException &e)
    {
        LOG_ERROR << "RedisException: " << e.message();
        throw BusinessException(ErrorCode::CACHE_ERROR(), "RedisException:" + e.message());
    }
}

bool ShopServiceImpl::tryLock(const std::string &key)
{
    LOG_DEBUG << "[tryLock] begin";

    //创建锁，默认超时时间为10秒
    Command setCmd = Command::set(key, "1", false, Poco::Timespan(RedisConstants::LOCK_SHOP_TTL, 0));
    std::string result = _redisClientPtr->execute<std::string>(setCmd);
    bool success = (result.compare("OK") == 0);

    LOG_INFO << "[tryLock] key:" << key << ",success:" << success;

    LOG_DEBUG << "[tryLock] end";
    return success;
}

void ShopServiceImpl::unlock(const std::string &key)
{
    LOG_DEBUG << "[unlock] begin";

    _redisClientPtr->execute<Poco::Int64>(Command::del(key));

    LOG_DEBUG << "[unlock] end";
};

void ShopServiceImpl::setCurrentId(long id)
{
    _curId = id;
}
long ShopServiceImpl::getCurrentId()
{
    return _curId;
}

void ShopServiceImpl::setWithLogicalExpire()
{
    LOG_DEBUG << "[setWithLogicalExpire] begin";

    long id = getCurrentId();
    std::string key = RedisConstants::CACHE_SHOP_KEY + std::to_string(id);
    long time = RedisConstants::CACHE_SHOP_TTL;
    LOG_INFO << "[setWithLogicalExpire] key" << key << ",id" << id << ",time" << time;

    // 查询数据库
    TbShop shop = queryDbById(id);
    // 重建缓存
    // 设置逻辑过期
    Poco::LocalDateTime now;
    Poco::Timespan t(time, 0);
    RedisData<TbShop> redisData(shop, now + t);

    // 写入Redis
    std::map<std::string, std::string> shopMap;
    shopMap = JsonUtils::ConvertJsonToMap(redisData.toJsonStr());
    Command redisCmd = Command::hmset(key, shopMap);
    _redisClientPtr->execute<std::string>(redisCmd);

    LOG_DEBUG << "[setWithLogicalExpire] end";
}

//初始化时，构建缓存
void ShopServiceImpl::buildLogicalExpireCache(const std::string &key, long id)
{
    if (!isLogicalExpire)
    {
        isLogicalExpire = true;
        // 查询数据库
        TbShop shop = queryDbById(id);
        Poco::LocalDateTime now;
        RedisData<TbShop> redisData(shop, now);

        // 写入Redis
        std::map<std::string, std::string> shopMap;
        shopMap = JsonUtils::ConvertJsonToMap(redisData.toJsonStr());
        Command redisCmd = Command::hmset(key, shopMap);
        _redisClientPtr->execute<std::string>(redisCmd);
    }
}

TbShop ShopServiceImpl::getById(long id)
{
    // 1.校验商铺Id合法性
    if (id <= 0)
    {
        // 2.如果不符合，返回错误信息
        throw BusinessException(ErrorCode::PARAMS_ERROR(), "入参中的商铺ID不合法");
    }

    // 3.从Redis查询商铺缓存信息
    std::string shopKey = RedisConstants::CACHE_SHOP_KEY + std::to_string(id);

    LOG_DEBUG << "shopKey=" << shopKey;

    std::map<std::string, std::string> shopMap;

    Command redisCmd = Command::exists(shopKey);

    if (_redisClientPtr->execute<Poco::Int64>(redisCmd) == 1)
    {
        redisCmd = Command::hget(shopKey, RedisConstants::ORIGIN_DATA_KEY);

        BulkString result = _redisClientPtr->execute<BulkString>(redisCmd);

        // LOG_DEBUG << "origin=" << (result.isNull())?:"NULL":cacheCode.value();

        Json::Reader reader;
        Json::Value value;
        if (reader.parse(result.value(), value))
        {
            return TbShop(value);
        }
        // todo，后续考虑如何将json转换成对象
        /*
        redisCmd = Command::hgetall(shopKey);

        Array result = _redisClientPtr->execute<Array>(redisCmd);

        for (int i = 0; i < result.size() - 1;)
        {
            std::string key = (result.get<BulkString>(i)).value();
            std::string val = (result.get<BulkString>(i + 1)).value();
            shopMap[key] = val;
            i = i + 2;
        }
        return TbShop(ConvertMapToJson(shopMap));
        */
    }

    // 4. Redis缓存不在，则根据Id查询数据库信息
    auto shop = shopMapper.findOne(Criteria(TbShop::Cols::_id, CompareOperator::EQ, std::to_string(id)));

    // 5. 不存在
    if (shop.getValueOfId() <= 0)
    {
        throw BusinessException(ErrorCode::PARAMS_ERROR(), "商铺ID: " + std::to_string(id) + " 不存在");
    }

    // 6. 存入Redis缓存, 存储时，将原始的shop的信息封装到origin，便于后续从缓存取时，直接解析为Json::Value对象
    Json::Value jsonValue = shop.toJson();
    jsonValue[RedisConstants::ORIGIN_DATA_KEY] = jsonValue.toStyledString();
    shopMap = JsonUtils::ConvertJsonToMap(jsonValue);

    redisCmd.clear();
    redisCmd = Command::hmset(shopKey, shopMap);
    _redisClientPtr->execute<std::string>(redisCmd);

    return shop;
}

TbShop ShopServiceImpl::queryDbById(long id)
{
    LOG_DEBUG << "queryDbById Id: " << id;
    //加try catch
    // 4. Redis缓存不在，则根据Id查询数据库信息
    try
    {
        auto shop = shopMapper.findOne(Criteria(TbShop::Cols::_id, CompareOperator::EQ, std::to_string(id)));
        return shop;
    }
    catch (DrogonDbException &e)
    {
        std::string errMsg = std::string("DrogonDbException:") + e.base().what();
        LOG_ERROR << errMsg;
        // return nullptr;
        throw BusinessException(ErrorCode::DB_ERROR(), errMsg);
    }
}

long ShopServiceImpl::save(const TbShop &shop)
{
    std::string id = std::to_string(shop.getValueOfId());

    auto ret = shopMapper.findBy(Criteria(TbShop::Cols::_id, CompareOperator::EQ, id));

    if (ret.size() == 1)
    {
        throw BusinessException(ErrorCode::PARAMS_ERROR(), "新增商铺id:" + id + "已存在,添加失败");
    }

    shopMapper.insert(const_cast<TbShop &>(shop)); // todo  新增id有BUG,会默认增加，而不是增加输入的id，跟自增有关

    return 0;
}

long ShopServiceImpl::update(const TbShop &shop)
{
    long id = shop.getValueOfId();

    if (id <= 0)
    {
        throw BusinessException(ErrorCode::PARAMS_ERROR(), "要更新的商铺ID不存在");
    }

    // 1. 先操作数据库
    shopMapper.update(shop);

    // 2. 再更新缓存
    std::string shopKey = RedisConstants::CACHE_SHOP_KEY + std::to_string(id);
    _redisClientPtr->execute<Poco::Int64>(Command::del(shopKey));

    return 0;
}

void ShopServiceImpl::loadShopData()
{
    LOG_DEBUG << "ShopServiceImpl::loadShopData begin";

    // 1.查询店铺信息
    try
    {
        auto shops = shopMapper.findAll();

        // 2.把店铺分组，按照typeId分组，typeId一致的放到一个集合
        std::map<long, std::vector<TbShop>> _map;
        for (auto shop : shops)
        {
            long typeId = shop.getValueOfTypeId();
            if (_map.count(typeId) > 0)
            {
                _map[typeId].push_back(shop);
            }
            else
            {
                std::vector<TbShop> tmp;
                tmp.push_back(shop);
                _map[typeId] = tmp;
            }
        }

        // 3.分批完成写入Redis
        for (auto iter = _map.begin(); iter != _map.end(); iter++)
        {
            // 3.1.获取类型id
            long typeId = iter->first;
            std::string key = RedisConstants::SHOP_GEO_KEY + std::to_string(typeId);
            // 3.2.获取同类型的店铺的集合
            std::vector<TbShop> value = iter->second;
            // 3.3.写入redis GEOADD key 经度 纬度 member
            Poco::Redis::Array command;
            command.add("GEOADD").add(key);

            for (auto shop : value)
            {
                command.add(std::to_string(shop.getValueOfX())).add(std::to_string(shop.getValueOfY())).add(std::to_string(shop.getValueOfId()));
            }
            LOG_DEBUG << "ShopServiceImpl::loadShopData cmdVal: ";
            Poco::Int64 result = _redisClientPtr->execute<Poco::Int64>(command);
        }
    }
    catch (DrogonDbException &e)
    {
        std::string errMsg = std::string("DrogonDbException:") + e.base().what();
        LOG_ERROR << errMsg;
        // return nullptr;
        throw BusinessException(ErrorCode::DB_ERROR(), errMsg);
    }
    catch (Poco::Redis::RedisException &e)
    {
        LOG_ERROR << "RedisException: " << e.message();
        throw BusinessException(ErrorCode::CACHE_ERROR(), "RedisException:" + e.message());
    }

    LOG_DEBUG << "ShopServiceImpl::loadShopData end";
}

std::vector<TbShop> ShopServiceImpl::queryByType(Json::Value jsonObj)
{
    long typeId = atol((jsonObj["typeId"].asString()).c_str());
    long current = atol(((jsonObj.isMember("current")) ? jsonObj["current"].asString() : "0").c_str());
    double x = atof(((jsonObj.isMember("x")) ? jsonObj["x"].asString() : "-1").c_str());
    double y = atof(((jsonObj.isMember("y")) ? jsonObj["y"].asString() : "-1").c_str());

    std::vector<TbShop> resShops;

    loadShopData();
    int DEFAULT_PAGE_SIZE = 5;
    // 1.判断是否需要根据坐标查询
    if (x == -1 || y == -1)
    {
        // 不需要坐标查询，按数据库查询
        auto shops = shopMapper.findBy(Criteria(TbShop::Cols::_type_id, CompareOperator::EQ, std::to_string(typeId)));

        //分页查询  current, SystemConstants.DEFAULT_PAGE_SIZE
        int shopSize = shops.size();
        int defaultPage = SystemConstants::DEFAULT_PAGE_SIZE;
        int end = std::min(shopSize, defaultPage);

        for (int i = current; i < end; i++)
        {
            resShops.push_back(shops[i]);
        }

        // 返回数据
        return resShops;
    }

    // 2.计算分页参数
    int from = (current - 1) * SystemConstants::DEFAULT_PAGE_SIZE;
    int end = current * SystemConstants::DEFAULT_PAGE_SIZE;

    // 3.查询redis、按照距离排序、分页。结果：shopId、distance
    std::string key = RedisConstants::SHOP_GEO_KEY + std::to_string(typeId);
    std::string distance = "5000";
    // GEOSEARCH key BYLONLAT x y BYRADIUS 10 WITHDISTANCE
    // GEOSEARCH shop:geo:1 FROMLONLAT 120.149993 30.334229 BYRADIUS 5000 km WITHDIST
    Poco::Redis::Array command;
    command.add("GEOSEARCH").add(key).add("FROMLONLAT").add(std::to_string(x)).add(std::to_string(y)).add("BYRADIUS").add(distance).add("km").add("WITHDIST");
    LOG_DEBUG << "ShopServiceImpl::queryByType command=" << command.toString();
    //.limit(end)
    Poco::Redis::Array result = _redisClientPtr->execute<Poco::Redis::Array>(command);
    LOG_DEBUG << "ShopServiceImpl::queryByType result.size=" << result.size();
    // 4.解析出id
    if (result.size() == 0)
    {
        return resShops;
    }

    if (result.size() / 2 <= from)
    {
        // 没有下一页了，结束
        return resShops;
    }
    // 4.1.截取 from ~ end的部分的shopId
    std::vector<std::string> ids;
    std::map<std::string, std::string> distanceMap;
    for (int i = from; i < end; i++)
    {
        // 4.2.获取店铺id
        // 4.3.获取距离
        // TODO  Bad cast exception  std::to_string
        std::string id = result.get<BulkString>(i).value();
        std::string distance = result.get<BulkString>(i + 1).value();
        ids.push_back(id);
        distanceMap[id] = distance;
    }

    // 5.根据id查询Shop
    /*
    String idStr = StrUtil.join(",", ids);
    List<Shop> shops = query().in("id", ids).last("ORDER BY FIELD(id," + idStr + ")").list();
    for (Shop shop : shops)
    {
        shop.setDistance(distanceMap.get(shop.getId().toString()).getValue());
    }
    */
    // 6.返回
    return resShops;
}
