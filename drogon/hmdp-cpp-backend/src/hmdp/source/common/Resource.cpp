#include <common/Resource.h>

using namespace Poco::Redis;
using Poco::Redis::Array;
using Poco::Redis::BulkString;

bool Resource::_isConnectRedis = false;
bool Resource::_isConnectMySQL = false;
// RedisClientPtr Resource::_redisClientPtr(new Poco::Redis::Client());
SessionPoolPtr Resource::_sessionPoolPtr = nullptr;
RedisClientPtr Resource::_redisClientPtr = nullptr;
ThreadPoolPtr Resource::_threadPoolPtr = nullptr;

Resource::Resource()
{
}
Resource::~Resource()
{
}

void Resource::loadConfigFile(const std::string &fileName)
{
    _yamlConfig = YAML::LoadFile(fileName);
    load();
}

void Resource::load()
{
    loadMySQL(_yamlConfig["mysql"]);
    loadRedis(_yamlConfig["redis"]);
    loadThreadPool(_yamlConfig["threadpool"]);
}

void Resource::loadMySQL(const YAML::Node &mysql)
{
    if (!mysql)
    {
        std::cout << "[loadMySQL] fail" << std::endl;
        return;
    }

    auto host = mysql["host"].as<std::string>();
    auto port = mysql["port"].as<int>();
    auto dbname = mysql["dbname"].as<std::string>();
    auto user = mysql["user"].as<std::string>();
    auto password = mysql["password"].as<std::string>();
    auto number_of_connections = mysql["number_of_connections"].as<int>();
    // createMySQLClient

    //待解决mysqlclient库加载的问题
    try
    {
        Poco::Data::MySQL::Connector::registerConnector();

        std::string name = "MySQL";
        std::string connectionString = "host=" + host + ";port=" + std::to_string(port) + ";db=" + dbname + ";user=" + user + ";password=" + password + ";compress=true;auto-reconnect=true";

        _sessionPoolPtr = std::make_shared<Poco::Data::SessionPool>(name, connectionString, number_of_connections);

        Poco::Data::Session session = _sessionPoolPtr->get();
        if (!session.isConnected())
        {
            std::cout << "[loadMySQL] session is not connected!!!" << std::endl;
        }
        else
        {
            _isConnectMySQL = true;
            std::cout << "[loadMySQL] Connected to [" << host << ':' << port << ']' << ", connected OK." << std::endl;
        }

        /*
        //TEST
        Poco::Data::Statement sql_select(session);
        sql_select << "select * from tb_voucher_order";
        Poco::Data::RecordSet res(sql_select);
        sql_select.execute();

        bool more = res.moveFirst();
        while (more)
        {
            std::cout << res[0].convert<std::string>();
            more = res.moveNext();
        }
        */
    }
    catch (const Poco::Data::DataException &e)
    {
        std::cout << "[loadMySQL] Couldn't connect  [" << host << ':' << port << ']' << e.message() << std::endl;
    }
}

void Resource::loadRedis(const YAML::Node &redis)
{
    if (!redis)
    {
        std::cout << "[loadRedis] fail" << std::endl;
        return;
    }

    auto host = redis["host"].as<std::string>();
    auto port = redis["port"].as<int>();
    auto db = redis["db"].as<std::string>();
    auto user = redis["user"].as<std::string>();
    auto password = redis["password"].as<std::string>();
    auto number_of_connections = redis["number_of_connections"].as<int>();

    //_redisClientPtr = new Poco::Redis::Client();
    _redisClientPtr = std::make_shared<Poco::Redis::Client>();
    try
    {
        _redisClientPtr->connect(host, port);

        Command auth = Command::auth(password);
        std::string result = _redisClientPtr->execute<std::string>(auth);
        if (result.compare("OK") == 0)
        {
            _isConnectRedis = true;
            std::cout << "[loadRedis] Connected to [" << host << ':' << port << ']' << ", auth OK." << std::endl;
        }
    }
    catch (Poco::Redis::RedisException &e)
    {
        std::cout << "[loadRedis] Couldn't connect  [" << host << ':' << port << ']' << e.message() << std::endl;
    }
}

RedisClientPtr Resource::getRedisInstance()
{
    if (_isConnectRedis)
    {
        return _redisClientPtr;
    }

    return nullptr;
}

SessionPoolPtr Resource::getMySQLInstance()
{
    if (_isConnectMySQL)
    {
        return _sessionPoolPtr;
    }

    return nullptr;
}

ThreadPoolPtr Resource::getThreadPoolInstance()
{
    return _threadPoolPtr;
}

void Resource::loadThreadPool(const YAML::Node &threadPool)
{
    if (!threadPool)
    {
        std::cout << "[loadThreadPool] fail" << std::endl;
        return;
    }

    auto min = threadPool["min"].as<int>();
    auto max = threadPool["max"].as<int>();
    auto idletime = threadPool["idletime"].as<int>();

    _threadPoolPtr = std::make_shared<Poco::ThreadPool>(min, max, idletime);

    std::cout << "[loadThreadPool]: " << min << std::endl;
    // createThreadPool
}