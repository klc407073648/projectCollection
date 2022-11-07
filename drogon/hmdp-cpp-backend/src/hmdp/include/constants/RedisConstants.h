#include <string>

#pragma once
/**
* Redis常量类
*/
class RedisConstants
{
public:
    //"const std::string" 类型的成员不能包含类内初始值设定项
    //下述TTL超时时间均以秒为单位
    static const std::string LOGIN_CODE_KEY;
    static const long LOGIN_CODE_TTL = 2 * 60; // 120sec

    static const std::string LOGIN_USER_KEY;
    static const long LOGIN_USER_TTL = 10 * 60 * 60;

    static const long CACHE_NULL_TTL = 2 * 60;

    static const long CACHE_SHOP_TTL = 30;
    static const std::string CACHE_SHOP_KEY;

    static const std::string LOCK_SHOP_KEY;
    static const long LOCK_SHOP_TTL = 10;

    static const std::string LOCK_ORDER_KEY;
    static const long LOCK_ORDER_TTL = 10;

    static const std::string SECKILL_STOCK_KEY;
    static const std::string BLOG_LIKED_KEY;
    static const std::string FEED_KEY;
    static const std::string SHOP_GEO_KEY;
    static const std::string USER_SIGN_KEY;

    static const std::string ORIGIN_DATA_KEY; //存储JSON串的原始数据的标识
    static const std::string REDIS_DATA_EXPIRETIME_KEY; //存储JSON串的原始数据的标识
};