#pragma once

#include <yaml-cpp/yaml.h> 
#include "Poco/Redis/Redis.h"
#include "Poco/Redis/Client.h"
#include "Poco/Exception.h"
#include "Poco/Delegate.h"
#include "Poco/Thread.h"
#include "Poco/Redis/AsyncReader.h"
#include "Poco/Redis/Command.h"
#include "Poco/Redis/PoolableConnectionFactory.h"
#include "Poco/ThreadPool.h"
#include "Poco/Data/MySQL/Connector.h"
#include "Poco/Data/SessionPool.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/Data/Column.h"
#include <iostream>

/**
 * 全局资源管理类
 */
using ThreadPoolPtr = std::shared_ptr<Poco::ThreadPool>;
using RedisClientPtr = std::shared_ptr<Poco::Redis::Client>;
using SessionPoolPtr = std::shared_ptr<Poco::Data::SessionPool>;

class Resource
{
public:
   Resource();
   ~Resource();
   void loadConfigFile(const std::string& fileName);
   static SessionPoolPtr getMySQLInstance();
   static RedisClientPtr getRedisInstance();
   static ThreadPoolPtr getThreadPoolInstance();
private:
    void load();
    void loadMySQL(const YAML::Node &mysql);
    void loadRedis(const YAML::Node &redis);
    void loadThreadPool(const YAML::Node &threadPool);
private:
    YAML::Node _yamlConfig;
    static bool _isConnectRedis;
    static bool _isConnectMySQL;
    static SessionPoolPtr _sessionPoolPtr;
	static RedisClientPtr _redisClientPtr;
    static ThreadPoolPtr _threadPoolPtr;
};
