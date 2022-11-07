#pragma once

#include <iostream>
#include <string>
#include <json/json.h>
#include <Poco/LocalDateTime.h>
#include <Poco/DateTime.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeParser.h>

using Poco::LocalDateTime;
using Poco::DateTime;
using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::DateTimeParser;

template <class R>
class RedisData
{
public:
    RedisData()
    {
    }

    RedisData(R data, LocalDateTime time)
        : data_(data), time_(time)
    {
    }

    R getData()
    {
        return data_;
    }

    const LocalDateTime &getExpireTime()
    {
        return time_;
    }

    void setData(R data)
    {
        data_ = data;
    }

    void setExpireTime(LocalDateTime time)
    {
        time_ = time;
    }

    Json::Value toJsonStr()
    {
        Json::Value value;
        value = data_.toJson();
        value["OriginData"] = data_.toJson().toStyledString();
        value["ExpireTime"] = (Json::Value(DateTimeFormatter::format(time_, DateTimeFormat::SORTABLE_FORMAT))).toStyledString();
        
        std::cout<<"[toJsonStr] " << value.toStyledString();
        return value;
    }

    void fromJsonStr(Json::Value originData,Json::Value expireTime)
    { 
        std::cout<<"[fromJsonStr] originData:" << originData.toStyledString();
		std::cout<<"[fromJsonStr] expireTime:" << expireTime.toStyledString();
        
        data_ = R(originData);

        //从DateTimeFormat::SORTABLE_FORMAT 2022-08-30 23:11:32格式转换成LocalDateTime
        DateTime dt;
        int tzd;
        DateTimeParser::parse(DateTimeFormat::SORTABLE_FORMAT, expireTime.toStyledString(), dt, tzd);
        dt.makeUTC(tzd);
        LocalDateTime ldt(tzd, dt);

        time_ = ldt;
    }

private:
    LocalDateTime time_;
    R data_;
};