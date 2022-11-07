#pragma once

#include <memory>
#include <string>
#include <models/TbShop.h>
#include <common/BaseResponse.h>
#include <common/ErrorCode.h>
#include <exception/BusinessException.h>
#include "ShopService.h"
#include "Poco/Redis/Redis.h"
#include "Poco/Redis/Client.h"
#include "Poco/Exception.h"
#include "Poco/Delegate.h"
#include "Poco/Thread.h"
#include "Poco/Redis/AsyncReader.h"
#include "Poco/Redis/Command.h"
#include "Poco/Redis/PoolableConnectionFactory.h"
#include "Poco/RunnableAdapter.h"
#include <common/Resource.h>
#include <common/RedisData.h>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::hmdp;

namespace hmdp
{

  class ShopServiceImpl : public ShopService
  {
  public:
    ShopServiceImpl();

    ~ShopServiceImpl();

    /**
     * @brief 根据id查询商铺：
     * 1. 先查询Redis缓存，命中直接返回；
     * 2. 未命中，查询数据库；
     * 3. 数据库查询命中，刷新缓存，同时返回
     * 4. 数据库未命中，则返回商铺不存在的报错
     *
     * @param id
     * @return std::string
     */
    TbShop queryById(long id, CacheType type = DEFAULT) override;

    long save(const TbShop &tbShop) override;

    long update(const TbShop &tbShop) override;

    std::vector<TbShop> queryByType(Json::Value jsonObj) override;

    void setCurrentId(long id);
    long getCurrentId();

  private:
    void loadShopData();
    TbShop queryWithPassThrough(long id);
    TbShop queryWithMutex(long id);
    TbShop queryWithLogicalExpire(long id);

    bool tryLock(const std::string &key);
    void unlock(const std::string &key);
    void setWithLogicalExpire(); // const std::string &key, long id, long time

    TbShop getById(long id);
    TbShop queryDbById(long id);

    Json::Value queryShopCache(const std::string &key, const std::string &field);
    void writeShopCache(const std::string &key, const std::string &field, const TbShop &shop, long expireTime);
    void writeNullCache(const std::string &key, long expireTime);

    void buildLogicalExpireCache(const std::string &key, long id);

  private:
    Mapper<TbShop> shopMapper = Mapper<TbShop>(app().getDbClient());  //对象持久化映射层,连接User对象和数据库
    RedisClientPtr _redisClientPtr = Resource::getRedisInstance();    // Redis连接对象
    ThreadPoolPtr _threadPoolPtr = Resource::getThreadPoolInstance(); // Redis连接对象
    long _curId;
    bool isLogicalExpire = false;
  };

}