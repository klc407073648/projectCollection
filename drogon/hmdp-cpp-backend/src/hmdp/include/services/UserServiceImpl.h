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
#include "UserService.h"
#include "Poco/Redis/Redis.h"
#include "Poco/Redis/Client.h"
#include "Poco/Redis/Array.h"
#include "Poco/Redis/Command.h"
#include "Poco/Exception.h"
#include<common/Resource.h>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::hmdp;

namespace hmdp
{
  class UserServiceImpl : public UserService
  {
  public:
    UserServiceImpl();

    ~UserServiceImpl();

    std::string sendCode(const std::string &phone) override;

    std::string login(const std::string &phone, const std::string &code) override;

    long logout(const HttpRequestPtr &request) override;

    TbUser getCurrent(const HttpRequestPtr &request) override;

  private:
    TbUser createUserWithPhone(const std::string &phone);
    TbUser getSafetyUser(const TbUser& originUser);

  private:
    Mapper<TbUser> userMapper = Mapper<TbUser>(app().getDbClient()); //对象持久化映射层,连接User对象和数据库
    RedisClientPtr _redisClientPtr = Resource::getRedisInstance();            // Redis连接对象
  };
}