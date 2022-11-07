#pragma once

#include <memory>
#include <string>
#include <trantor/utils/Utilities.h>
#include <models/TbVoucher.h>
#include <models/TbSeckillVoucher.h>
#include <common/BaseResponse.h>
#include <common/ErrorCode.h>
#include <utils/RegexUtils.h>
#include <utils/RandomUtils.h>
#include <exception/BusinessException.h>
#include <constants/RedisConstants.h>
#include "VoucherService.h"
#include "Poco/Redis/Redis.h"
#include "Poco/Redis/Client.h"
#include "Poco/Redis/Array.h"
#include "Poco/Redis/Command.h"
#include "Poco/Exception.h"
#include <common/Resource.h>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::hmdp;

namespace hmdp
{
  class VoucherServiceImpl : public VoucherService
  {
  public:
    VoucherServiceImpl();

    ~VoucherServiceImpl();

    void addSeckillVoucher(TbVoucher &voucher,Json::Value jsonObj)  override;
    void addVoucher(TbVoucher &voucher,Json::Value jsonObj)   override;
    std::vector<TbVoucher> queryVoucherOfShop(long shopId) override;

  private:
  private:
    Mapper<TbVoucher> voucherMapper = Mapper<TbVoucher>(app().getDbClient()); //对象持久化映射层,连接User对象和数据库
    Mapper<TbSeckillVoucher> seckillVoucherMapper = Mapper<TbSeckillVoucher>(app().getDbClient()); //对象持久化映射层,连接User对象和数据库
    RedisClientPtr _redisClientPtr = Resource::getRedisInstance();             // Redis连接对象
  };
}