#pragma once

#include <memory>
#include <string>
#include <trantor/utils/Utilities.h>
#include <models/TbUser.h>
#include <common/BaseResponse.h>
#include <common/ErrorCode.h>
#include <utils/RegexUtils.h>
#include <utils/RandomUtils.h>
#include <exception/BusinessException.h>
#include <constants/RedisConstants.h>
#include "VoucherOrderService.h"
#include "Poco/Redis/Redis.h"
#include "Poco/Redis/Client.h"
#include "Poco/Redis/Array.h"
#include "Poco/Redis/Command.h"
#include "Poco/Exception.h"
#include <common/Resource.h>
#include "Poco/UUID.h"
#include "Poco/UUIDGenerator.h"
#include <Poco/LocalDateTime.h>
#include <Poco/DateTime.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeParser.h>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::hmdp;

namespace hmdp
{
  class VoucherOrderServiceImpl : public VoucherOrderService
  {
  public:
    VoucherOrderServiceImpl();

    ~VoucherOrderServiceImpl();

    long seckillVoucher(long voucherId,TbUser loginUser) override;

  private:
    long createVoucherOrder(long voucherId, TbUser loginUser);
    bool tryLock(const std::string &key, const std::string &val, const Poco::Timespan &expireTime);
    void unLock(const std::string &key);
  private:
    Mapper<TbSeckillVoucher> seckillVoucherMapper = Mapper<TbSeckillVoucher>(app().getDbClient()); //对象持久化映射层,连接User对象和数据库
    Mapper<TbVoucherOrder> voucherOrderMapper = Mapper<TbVoucherOrder>(app().getDbClient());
    SessionPoolPtr _sessionPoolPtr = Resource::getMySQLInstance();
    RedisClientPtr _redisClientPtr = Resource::getRedisInstance();                       // Redis连接对象
  };
}