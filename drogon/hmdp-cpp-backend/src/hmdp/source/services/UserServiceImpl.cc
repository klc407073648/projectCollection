#include <services/UserServiceImpl.h>
#include <exception>

using namespace hmdp;
using namespace drogon::internal;
using namespace drogon::nosql;
using namespace Poco::Redis;
using Poco::Redis::Array;
using Poco::Redis::BulkString;

UserServiceImpl::UserServiceImpl()
{
    
}

UserServiceImpl::~UserServiceImpl()
{
   
}

std::string UserServiceImpl::sendCode(const std::string &phone)
{
    LOG_DEBUG << "发送短信验证码对应的手机号:" << phone;

    try
    {
        // 1.校验手机号
        if (!RegexUtils::isPhoneInvalid(phone))
        {
            // 2.如果不符合，返回错误信息
            throw BusinessException(ErrorCode::PARAMS_ERROR(), "入参中手机号格式不对");
        }

        // 3.符合，生成验证码
        std::string code = RandomUtils::getRandomNumberStr(6);

        // 4.保存验证码到session
        std::string key = RedisConstants::LOGIN_USER_KEY + phone;

        // SET KEY CODE EX LOGIN_CODE_TTL
        Command setex = Command::set(key, code, true, Poco::Timespan(RedisConstants::LOGIN_CODE_TTL, 0));
        _redisClientPtr->execute<std::string>(setex);

        LOG_DEBUG << "保存验证码到session: " << key << "=" << code;

        // 5.发送验证码
        LOG_INFO << "发送短信验证码成功，验证码:" << code;

        // 返回ok
        return code;
    }
    catch (Poco::Redis::RedisException &e)
    {
        LOG_ERROR << "RedisException: " << e.message();
        throw BusinessException(ErrorCode::CACHE_ERROR(), "RedisException:" + e.message());
    }
}

std::string UserServiceImpl::login(const std::string &phone, const std::string &code)
{
    LOG_DEBUG << "短信验证码登陆, 手机号: " << phone << ", 验证码: " << code;

    try
    {
        // 1.校验手机号
        if (!RegexUtils::isPhoneInvalid(phone))
        {
            // 2.如果不符合，返回错误信息
            throw BusinessException(ErrorCode::PARAMS_ERROR(), "入参中手机号格式不对");
        }

        // 3.从redis获取验证码并校验
        std::string key = RedisConstants::LOGIN_USER_KEY + phone;

        Command redisCmd = Command::get(key);
        BulkString cacheCode = _redisClientPtr->execute<BulkString>(redisCmd);
       
		//LOG_INFO << "Redis中存储的验证码: " << key << "=" << (cacheCode.isNull())?:"NULL":cacheCode.value();
        
        if (cacheCode.isNull() || cacheCode.value() != code)
        {
            throw BusinessException(ErrorCode::PARAMS_ERROR(), "验证码不一致");
        }

        // 4.一致，根据手机号查询用户 select * from tb_user where phone = ?
        // 不能用findOne接口会，查到数据为0时，会抛异常UnexpectedRows
        TbUser user;
        auto users = userMapper.findBy(Criteria(TbUser::Cols::_phone, CompareOperator::EQ, phone));

        int userSize = users.size();
        LOG_DEBUG << "根据手机号查询的用户数量: " << userSize;

        // 5.判断用户是否存在
        if (userSize == 0)
        {
            // 6.不存在，创建新用户并保存
            user = createUserWithPhone(phone);
        }
        else if (userSize == 1)
        {
            user = getSafetyUser(users[0]);
        }
        else
        {
            throw BusinessException(ErrorCode::SYSTEM_ERROR(), "查询的用户数量:" + std::to_string(userSize) + ",异常");
        }

        // 7.保存用户信息到 redis中
        // 7.1.随机生成token，作为登录令牌
        std::string token = utils::getUuid();

        // 7.2.将User对象转为HashMap存储
        UserDTO *userDTO = new UserDTO(&user);
        std::map<std::string, std::string> userMap = userDTO->getMapObject();

        // 7.3.存储
        std::string tokenKey = RedisConstants::LOGIN_USER_KEY + token;
        redisCmd.clear();
        redisCmd = Command::hmset(tokenKey, userMap);
        _redisClientPtr->execute<std::string>(redisCmd);

        // 7.4.设置token有效期
        redisCmd.clear();
        redisCmd = Command::expire(tokenKey, RedisConstants::LOGIN_USER_TTL);
        _redisClientPtr->execute<Poco::Int64>(redisCmd);

        LOG_INFO << "tokenKey=" << tokenKey;

        // 8.返回token
        return token;
    }
    catch (Poco::Redis::RedisException &e)
    {
        LOG_ERROR << "RedisException: " << e.message();
        throw BusinessException(ErrorCode::CACHE_ERROR(), "RedisException:" + e.message());
    }
}

long UserServiceImpl::logout(const HttpRequestPtr &request)
{
}

TbUser UserServiceImpl::getCurrent(const HttpRequestPtr &request)
{
    try
    {
        // 1.获取请求头中的token
        std::string token = request->getHeader("JSESSIONID");

        LOG_DEBUG << "获取请求头中的token:" << token;

        if (token == "")
        {
            throw BusinessException(ErrorCode::NO_LOGIN(), "用户未登陆");
        }

        // 2.基于TOKEN获取redis中的用户
        std::string key = RedisConstants::LOGIN_USER_KEY + token;
        std::map<std::string, std::string> userMap;

        Command redisCmd = Command::hgetall(key);

        Array result = _redisClientPtr->execute<Array>(redisCmd);

        for (int i = 0; i < result.size() - 1;)
        {
            std::string key = (result.get<BulkString>(i)).value();
            std::string val = (result.get<BulkString>(i + 1)).value();
            userMap[key] = val;
            i = i + 2;
        }

        // 3.判断用户是否存在
        if (userMap.size() == 0)
        {
            throw BusinessException(ErrorCode::PARAMS_ERROR(), "用户不存在");
        }

        // 4.将查询到的hash数据转为UserDTO
        UserDTO *userDTO = new UserDTO(userMap);

        // 5.存在，保存用户信息到 ThreadLocal
        TbUser user;
        user.setId(userDTO->getId());
        user.setNickName(userDTO->getNickName());
        user.setIcon(userDTO->getIcon());

        // 6.刷新token有效期
        redisCmd.clear();
        redisCmd = Command::expire(key, RedisConstants::LOGIN_USER_TTL);
        _redisClientPtr->execute<Poco::Int64>(redisCmd);

        LOG_DEBUG << "刷新token时间: key:" << key << ",userId:" << user.getValueOfId();

        return user;
    }
    catch (Poco::Redis::RedisException &e)
    {
        LOG_ERROR << "RedisException: " << e.message();
        throw BusinessException(ErrorCode::CACHE_ERROR(), "RedisException:" + e.message());
    }
}

TbUser UserServiceImpl::createUserWithPhone(const std::string &phone)
{
    TbUser user;
    user.setPhone(phone);
    user.setNickName("user_" + RandomUtils::getRandomNumberStr(10));
    userMapper.insert(user);

    auto ret = userMapper.findBy(Criteria(TbUser::Cols::_phone, CompareOperator::EQ, phone));

    if (ret.size() != 1)
    {
        throw BusinessException(ErrorCode::PARAMS_ERROR(), "插入数据失败");
    }

    return user;
}

TbUser UserServiceImpl::getSafetyUser(const TbUser &originUser)
{
    TbUser safetyUser;
    safetyUser.setId(originUser.getValueOfId());
    safetyUser.setPhone(originUser.getValueOfPhone());
    safetyUser.setNickName(originUser.getValueOfNickName());
    safetyUser.setIcon(originUser.getValueOfIcon());

    return safetyUser;
}
