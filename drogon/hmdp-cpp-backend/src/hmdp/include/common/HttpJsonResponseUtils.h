#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <json/json.h>
#include <common/ResultUtils.h>
#include <common/Response2json.h>

/**
 * @brief  返回错误的Json信息
 * @tparam
 */
template <class T>
class HttpJsonResponseUtils
{
public:
    //返回错误的Json信息
    static Json::Value makeErrorJsonResp(T data)
    {
        auto base = ResultUtils<long>::error(data);
        auto json = Response2json<long>::rep2json(base);

        return json;
    }

    //返回正确的Json信息
    static Json::Value makeSuccessJsonResp(T data)
    {
        auto base = ResultUtils<T>::susscess(data);
        auto json = Response2json<T>::rep2json(base);

        return json;
    }
};



/**
 * @brief  HttpJsonResponseUtils 全特化 std::string
 *
 * @tparam
 */

/*
template <>
class HttpJsonResponseUtils<std::string>
{
public:
    static Json::Value makeSuccessJsonResp(std::string str)
    {
        auto base = ResultUtils<std::string>::susscess(str);
        auto json = Response2json<std::string>::rep2json(base);

        return json;
    }
};
*/

/**
 * @brief  HttpJsonResponseUtils 全特化 long
 *
 * @tparam
 */

/*
template <>
class HttpJsonResponseUtils<long>
{
public:
    static Json::Value makeSuccessJsonResp(long rep)
    {
        auto base = ResultUtils<long>::susscess(res);
        auto json = Response2json<long>::rep2json(base);

        return json;
    }
};
*/

/**
 * @brief  HttpJsonResponseUtils 全特化 Json::Value
 *
 * @tparam
 */

/*
template <>
class HttpJsonResponseUtils
{
public:
    static Json::Value makeSuccessJsonResp(Json::Value data)
    {
        auto base = ResultUtils<Json::Value>::susscess(data);
        auto json = Response2json<Json::Value>::rep2json(base);

        return json;
    }
};
*/