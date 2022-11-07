#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <json/json.h>
#include "BaseResponse.h"

/**
 * @brief Response2json 模板类
 *
 * @tparam T
 */
template <class T>
class Response2json
{
public:
    static Json::Value rep2json(BaseResponse<T> rep)
    {
        Json::Value ret;
        ret["code"] = rep.getCode();
        ret["data"] = rep.getData();
        ret["message"] = rep.getMessage();
        ret["description"] = rep.getDescription();
        return ret;
    }
};

/**
 * @brief  Response2json 全特化 std::vector<T>
 *
 * @tparam
 */
template <class T>
class Response2json<std::vector<T>>
{
public:
    static Json::Value rep2json(BaseResponse<std::vector<T>> rep)
    {
        Json::Value arr;
        for (auto object : rep.getData())
        {
            arr.append(object.toJson());
        }

        Json::Value ret;
        ret["code"] = rep.getCode();
        ret["data"] = arr;
        ret["message"] = rep.getMessage();
        ret["description"] = rep.getDescription();
        return ret;
    }
};

/**
 * @brief  Response2json 全特化 User
 *
 * @tparam
 */
template <>
class Response2json<long>
{
public:
    static Json::Value rep2json(BaseResponse<long> rep)
    {
        Json::Value ret;
        ret["code"] = rep.getCode();
        ret["data"] = static_cast<Json::Int64>(rep.getData());
        ret["message"] = rep.getMessage();
        ret["description"] = rep.getDescription();
        return ret;
    }
};
