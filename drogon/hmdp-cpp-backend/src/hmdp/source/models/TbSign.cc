/**
 *
 *  TbSign.cc
 *  DO NOT EDIT. This file is generated by drogon_ctl
 *
 */

#include <models/TbSign.h>
#include <drogon/utils/Utilities.h>
#include <string>

using namespace drogon;
using namespace drogon_model::hmdp;

const std::string TbSign::Cols::_id = "id";
const std::string TbSign::Cols::_user_id = "user_id";
const std::string TbSign::Cols::_year = "year";
const std::string TbSign::Cols::_month = "month";
const std::string TbSign::Cols::_date = "date";
const std::string TbSign::Cols::_is_backup = "is_backup";
const std::string TbSign::primaryKeyName = "id";
const bool TbSign::hasPrimaryKey = true;
const std::string TbSign::tableName = "tb_sign";

const std::vector<typename TbSign::MetaData> TbSign::metaData_={
{"id","uint64_t","bigint(20) unsigned",8,1,1,1},
{"user_id","uint64_t","bigint(20) unsigned",8,0,0,1},
{"year","std::string","year(4)",0,0,0,1},
{"month","int8_t","tinyint(2)",1,0,0,1},
{"date","::trantor::Date","date",0,0,0,1},
{"is_backup","uint8_t","tinyint(1) unsigned",1,0,0,0}
};
const std::string &TbSign::getColumnName(size_t index) noexcept(false)
{
    assert(index < metaData_.size());
    return metaData_[index].colName_;
}
TbSign::TbSign(const Row &r, const ssize_t indexOffset) noexcept
{
    if(indexOffset < 0)
    {
        if(!r["id"].isNull())
        {
            id_=std::make_shared<uint64_t>(r["id"].as<uint64_t>());
        }
        if(!r["user_id"].isNull())
        {
            userId_=std::make_shared<uint64_t>(r["user_id"].as<uint64_t>());
        }
        if(!r["year"].isNull())
        {
            year_=std::make_shared<std::string>(r["year"].as<std::string>());
        }
        if(!r["month"].isNull())
        {
            month_=std::make_shared<int8_t>(r["month"].as<int8_t>());
        }
        if(!r["date"].isNull())
        {
            auto daysStr = r["date"].as<std::string>();
            struct tm stm;
            memset(&stm,0,sizeof(stm));
            strptime(daysStr.c_str(),"%Y-%m-%d",&stm);
            time_t t = mktime(&stm);
            date_=std::make_shared<::trantor::Date>(t*1000000);
        }
        if(!r["is_backup"].isNull())
        {
            isBackup_=std::make_shared<uint8_t>(r["is_backup"].as<uint8_t>());
        }
    }
    else
    {
        size_t offset = (size_t)indexOffset;
        if(offset + 6 > r.size())
        {
            LOG_FATAL << "Invalid SQL result for this model";
            return;
        }
        size_t index;
        index = offset + 0;
        if(!r[index].isNull())
        {
            id_=std::make_shared<uint64_t>(r[index].as<uint64_t>());
        }
        index = offset + 1;
        if(!r[index].isNull())
        {
            userId_=std::make_shared<uint64_t>(r[index].as<uint64_t>());
        }
        index = offset + 2;
        if(!r[index].isNull())
        {
            year_=std::make_shared<std::string>(r[index].as<std::string>());
        }
        index = offset + 3;
        if(!r[index].isNull())
        {
            month_=std::make_shared<int8_t>(r[index].as<int8_t>());
        }
        index = offset + 4;
        if(!r[index].isNull())
        {
            auto daysStr = r[index].as<std::string>();
            struct tm stm;
            memset(&stm,0,sizeof(stm));
            strptime(daysStr.c_str(),"%Y-%m-%d",&stm);
            time_t t = mktime(&stm);
            date_=std::make_shared<::trantor::Date>(t*1000000);
        }
        index = offset + 5;
        if(!r[index].isNull())
        {
            isBackup_=std::make_shared<uint8_t>(r[index].as<uint8_t>());
        }
    }

}

TbSign::TbSign(const Json::Value &pJson, const std::vector<std::string> &pMasqueradingVector) noexcept(false)
{
    if(pMasqueradingVector.size() != 6)
    {
        LOG_ERROR << "Bad masquerading vector";
        return;
    }
    if(!pMasqueradingVector[0].empty() && pJson.isMember(pMasqueradingVector[0]))
    {
        dirtyFlag_[0] = true;
        if(!pJson[pMasqueradingVector[0]].isNull())
        {
            id_=std::make_shared<uint64_t>((uint64_t)pJson[pMasqueradingVector[0]].asUInt64());
        }
    }
    if(!pMasqueradingVector[1].empty() && pJson.isMember(pMasqueradingVector[1]))
    {
        dirtyFlag_[1] = true;
        if(!pJson[pMasqueradingVector[1]].isNull())
        {
            userId_=std::make_shared<uint64_t>((uint64_t)pJson[pMasqueradingVector[1]].asUInt64());
        }
    }
    if(!pMasqueradingVector[2].empty() && pJson.isMember(pMasqueradingVector[2]))
    {
        dirtyFlag_[2] = true;
        if(!pJson[pMasqueradingVector[2]].isNull())
        {
            year_=std::make_shared<std::string>(pJson[pMasqueradingVector[2]].asString());

        }
    }
    if(!pMasqueradingVector[3].empty() && pJson.isMember(pMasqueradingVector[3]))
    {
        dirtyFlag_[3] = true;
        if(!pJson[pMasqueradingVector[3]].isNull())
        {
            month_=std::make_shared<int8_t>((int8_t)pJson[pMasqueradingVector[3]].asInt64());
        }
    }
    if(!pMasqueradingVector[4].empty() && pJson.isMember(pMasqueradingVector[4]))
    {
        dirtyFlag_[4] = true;
        if(!pJson[pMasqueradingVector[4]].isNull())
        {
            auto daysStr = pJson[pMasqueradingVector[4]].asString();
            struct tm stm;
            memset(&stm,0,sizeof(stm));
            strptime(daysStr.c_str(),"%Y-%m-%d",&stm);
            time_t t = mktime(&stm);
            date_=std::make_shared<::trantor::Date>(t*1000000);
        }
    }
    if(!pMasqueradingVector[5].empty() && pJson.isMember(pMasqueradingVector[5]))
    {
        dirtyFlag_[5] = true;
        if(!pJson[pMasqueradingVector[5]].isNull())
        {
            isBackup_=std::make_shared<uint8_t>((uint8_t)pJson[pMasqueradingVector[5]].asUInt64());
        }
    }
}

TbSign::TbSign(const Json::Value &pJson) noexcept(false)
{
    if(pJson.isMember("id"))
    {
        dirtyFlag_[0]=true;
        if(!pJson["id"].isNull())
        {
            id_=std::make_shared<uint64_t>((uint64_t)pJson["id"].asUInt64());
        }
    }
    if(pJson.isMember("user_id"))
    {
        dirtyFlag_[1]=true;
        if(!pJson["user_id"].isNull())
        {
            userId_=std::make_shared<uint64_t>((uint64_t)pJson["user_id"].asUInt64());
        }
    }
    if(pJson.isMember("year"))
    {
        dirtyFlag_[2]=true;
        if(!pJson["year"].isNull())
        {
            year_=std::make_shared<std::string>(pJson["year"].asString());
        }
    }
    if(pJson.isMember("month"))
    {
        dirtyFlag_[3]=true;
        if(!pJson["month"].isNull())
        {
            month_=std::make_shared<int8_t>((int8_t)pJson["month"].asInt64());
        }
    }
    if(pJson.isMember("date"))
    {
        dirtyFlag_[4]=true;
        if(!pJson["date"].isNull())
        {
            auto daysStr = pJson["date"].asString();
            struct tm stm;
            memset(&stm,0,sizeof(stm));
            strptime(daysStr.c_str(),"%Y-%m-%d",&stm);
            time_t t = mktime(&stm);
            date_=std::make_shared<::trantor::Date>(t*1000000);
        }
    }
    if(pJson.isMember("is_backup"))
    {
        dirtyFlag_[5]=true;
        if(!pJson["is_backup"].isNull())
        {
            isBackup_=std::make_shared<uint8_t>((uint8_t)pJson["is_backup"].asUInt64());
        }
    }
}

void TbSign::updateByMasqueradedJson(const Json::Value &pJson,
                                            const std::vector<std::string> &pMasqueradingVector) noexcept(false)
{
    if(pMasqueradingVector.size() != 6)
    {
        LOG_ERROR << "Bad masquerading vector";
        return;
    }
    if(!pMasqueradingVector[0].empty() && pJson.isMember(pMasqueradingVector[0]))
    {
        if(!pJson[pMasqueradingVector[0]].isNull())
        {
            id_=std::make_shared<uint64_t>((uint64_t)pJson[pMasqueradingVector[0]].asUInt64());
        }
    }
    if(!pMasqueradingVector[1].empty() && pJson.isMember(pMasqueradingVector[1]))
    {
        dirtyFlag_[1] = true;
        if(!pJson[pMasqueradingVector[1]].isNull())
        {
            userId_=std::make_shared<uint64_t>((uint64_t)pJson[pMasqueradingVector[1]].asUInt64());
        }
    }
    if(!pMasqueradingVector[2].empty() && pJson.isMember(pMasqueradingVector[2]))
    {
        dirtyFlag_[2] = true;
        if(!pJson[pMasqueradingVector[2]].isNull())
        {
            year_=std::make_shared<std::string>(pJson[pMasqueradingVector[2]].asString());
        }
    }
    if(!pMasqueradingVector[3].empty() && pJson.isMember(pMasqueradingVector[3]))
    {
        dirtyFlag_[3] = true;
        if(!pJson[pMasqueradingVector[3]].isNull())
        {
            month_=std::make_shared<int8_t>((int8_t)pJson[pMasqueradingVector[3]].asInt64());
        }
    }
    if(!pMasqueradingVector[4].empty() && pJson.isMember(pMasqueradingVector[4]))
    {
        dirtyFlag_[4] = true;
        if(!pJson[pMasqueradingVector[4]].isNull())
        {
            auto daysStr = pJson[pMasqueradingVector[4]].asString();
            struct tm stm;
            memset(&stm,0,sizeof(stm));
            strptime(daysStr.c_str(),"%Y-%m-%d",&stm);
            time_t t = mktime(&stm);
            date_=std::make_shared<::trantor::Date>(t*1000000);
        }
    }
    if(!pMasqueradingVector[5].empty() && pJson.isMember(pMasqueradingVector[5]))
    {
        dirtyFlag_[5] = true;
        if(!pJson[pMasqueradingVector[5]].isNull())
        {
            isBackup_=std::make_shared<uint8_t>((uint8_t)pJson[pMasqueradingVector[5]].asUInt64());
        }
    }
}
                                                                    
void TbSign::updateByJson(const Json::Value &pJson) noexcept(false)
{
    if(pJson.isMember("id"))
    {
        if(!pJson["id"].isNull())
        {
            id_=std::make_shared<uint64_t>((uint64_t)pJson["id"].asUInt64());
        }
    }
    if(pJson.isMember("user_id"))
    {
        dirtyFlag_[1] = true;
        if(!pJson["user_id"].isNull())
        {
            userId_=std::make_shared<uint64_t>((uint64_t)pJson["user_id"].asUInt64());
        }
    }
    if(pJson.isMember("year"))
    {
        dirtyFlag_[2] = true;
        if(!pJson["year"].isNull())
        {
            year_=std::make_shared<std::string>(pJson["year"].asString());
        }
    }
    if(pJson.isMember("month"))
    {
        dirtyFlag_[3] = true;
        if(!pJson["month"].isNull())
        {
            month_=std::make_shared<int8_t>((int8_t)pJson["month"].asInt64());
        }
    }
    if(pJson.isMember("date"))
    {
        dirtyFlag_[4] = true;
        if(!pJson["date"].isNull())
        {
            auto daysStr = pJson["date"].asString();
            struct tm stm;
            memset(&stm,0,sizeof(stm));
            strptime(daysStr.c_str(),"%Y-%m-%d",&stm);
            time_t t = mktime(&stm);
            date_=std::make_shared<::trantor::Date>(t*1000000);
        }
    }
    if(pJson.isMember("is_backup"))
    {
        dirtyFlag_[5] = true;
        if(!pJson["is_backup"].isNull())
        {
            isBackup_=std::make_shared<uint8_t>((uint8_t)pJson["is_backup"].asUInt64());
        }
    }
}

const uint64_t &TbSign::getValueOfId() const noexcept
{
    const static uint64_t defaultValue = uint64_t();
    if(id_)
        return *id_;
    return defaultValue;
}
const std::shared_ptr<uint64_t> &TbSign::getId() const noexcept
{
    return id_;
}
void TbSign::setId(const uint64_t &pId) noexcept
{
    id_ = std::make_shared<uint64_t>(pId);
    dirtyFlag_[0] = true;
}



const typename TbSign::PrimaryKeyType & TbSign::getPrimaryKey() const
{
    assert(id_);
    return *id_;
}

const uint64_t &TbSign::getValueOfUserId() const noexcept
{
    const static uint64_t defaultValue = uint64_t();
    if(userId_)
        return *userId_;
    return defaultValue;
}
const std::shared_ptr<uint64_t> &TbSign::getUserId() const noexcept
{
    return userId_;
}
void TbSign::setUserId(const uint64_t &pUserId) noexcept
{
    userId_ = std::make_shared<uint64_t>(pUserId);
    dirtyFlag_[1] = true;
}




const std::string &TbSign::getValueOfYear() const noexcept
{
    const static std::string defaultValue = std::string();
    if(year_)
        return *year_;
    return defaultValue;
}
const std::shared_ptr<std::string> &TbSign::getYear() const noexcept
{
    return year_;
}
void TbSign::setYear(const std::string &pYear) noexcept
{
    year_ = std::make_shared<std::string>(pYear);
    dirtyFlag_[2] = true;
}
void TbSign::setYear(std::string &&pYear) noexcept
{
    year_ = std::make_shared<std::string>(std::move(pYear));
    dirtyFlag_[2] = true;
}




const int8_t &TbSign::getValueOfMonth() const noexcept
{
    const static int8_t defaultValue = int8_t();
    if(month_)
        return *month_;
    return defaultValue;
}
const std::shared_ptr<int8_t> &TbSign::getMonth() const noexcept
{
    return month_;
}
void TbSign::setMonth(const int8_t &pMonth) noexcept
{
    month_ = std::make_shared<int8_t>(pMonth);
    dirtyFlag_[3] = true;
}




const ::trantor::Date &TbSign::getValueOfDate() const noexcept
{
    const static ::trantor::Date defaultValue = ::trantor::Date();
    if(date_)
        return *date_;
    return defaultValue;
}
const std::shared_ptr<::trantor::Date> &TbSign::getDate() const noexcept
{
    return date_;
}
void TbSign::setDate(const ::trantor::Date &pDate) noexcept
{
    date_ = std::make_shared<::trantor::Date>(pDate.roundDay());
    dirtyFlag_[4] = true;
}




const uint8_t &TbSign::getValueOfIsBackup() const noexcept
{
    const static uint8_t defaultValue = uint8_t();
    if(isBackup_)
        return *isBackup_;
    return defaultValue;
}
const std::shared_ptr<uint8_t> &TbSign::getIsBackup() const noexcept
{
    return isBackup_;
}
void TbSign::setIsBackup(const uint8_t &pIsBackup) noexcept
{
    isBackup_ = std::make_shared<uint8_t>(pIsBackup);
    dirtyFlag_[5] = true;
}


void TbSign::setIsBackupToNull() noexcept
{
    isBackup_.reset();
    dirtyFlag_[5] = true;
}


void TbSign::updateId(const uint64_t id)
{
    id_ = std::make_shared<uint64_t>(id);
}

const std::vector<std::string> &TbSign::insertColumns() noexcept
{
    static const std::vector<std::string> inCols={
        "user_id",
        "year",
        "month",
        "date",
        "is_backup"
    };
    return inCols;
}

void TbSign::outputArgs(drogon::orm::internal::SqlBinder &binder) const
{
    if(dirtyFlag_[1])
    {
        if(getUserId())
        {
            binder << getValueOfUserId();
        }
        else
        {
            binder << nullptr;
        }
    }
    if(dirtyFlag_[2])
    {
        if(getYear())
        {
            binder << getValueOfYear();
        }
        else
        {
            binder << nullptr;
        }
    }
    if(dirtyFlag_[3])
    {
        if(getMonth())
        {
            binder << getValueOfMonth();
        }
        else
        {
            binder << nullptr;
        }
    }
    if(dirtyFlag_[4])
    {
        if(getDate())
        {
            binder << getValueOfDate();
        }
        else
        {
            binder << nullptr;
        }
    }
    if(dirtyFlag_[5])
    {
        if(getIsBackup())
        {
            binder << getValueOfIsBackup();
        }
        else
        {
            binder << nullptr;
        }
    }
}

const std::vector<std::string> TbSign::updateColumns() const
{
    std::vector<std::string> ret;
    if(dirtyFlag_[1])
    {
        ret.push_back(getColumnName(1));
    }
    if(dirtyFlag_[2])
    {
        ret.push_back(getColumnName(2));
    }
    if(dirtyFlag_[3])
    {
        ret.push_back(getColumnName(3));
    }
    if(dirtyFlag_[4])
    {
        ret.push_back(getColumnName(4));
    }
    if(dirtyFlag_[5])
    {
        ret.push_back(getColumnName(5));
    }
    return ret;
}

void TbSign::updateArgs(drogon::orm::internal::SqlBinder &binder) const
{
    if(dirtyFlag_[1])
    {
        if(getUserId())
        {
            binder << getValueOfUserId();
        }
        else
        {
            binder << nullptr;
        }
    }
    if(dirtyFlag_[2])
    {
        if(getYear())
        {
            binder << getValueOfYear();
        }
        else
        {
            binder << nullptr;
        }
    }
    if(dirtyFlag_[3])
    {
        if(getMonth())
        {
            binder << getValueOfMonth();
        }
        else
        {
            binder << nullptr;
        }
    }
    if(dirtyFlag_[4])
    {
        if(getDate())
        {
            binder << getValueOfDate();
        }
        else
        {
            binder << nullptr;
        }
    }
    if(dirtyFlag_[5])
    {
        if(getIsBackup())
        {
            binder << getValueOfIsBackup();
        }
        else
        {
            binder << nullptr;
        }
    }
}
Json::Value TbSign::toJson() const
{
    Json::Value ret;
    if(getId())
    {
        ret["id"]=(Json::UInt64)getValueOfId();
    }
    else
    {
        ret["id"]=Json::Value();
    }
    if(getUserId())
    {
        ret["user_id"]=(Json::UInt64)getValueOfUserId();
    }
    else
    {
        ret["user_id"]=Json::Value();
    }
    if(getYear())
    {
        ret["year"]=getValueOfYear();
    }
    else
    {
        ret["year"]=Json::Value();
    }
    if(getMonth())
    {
        ret["month"]=getValueOfMonth();
    }
    else
    {
        ret["month"]=Json::Value();
    }
    if(getDate())
    {
        ret["date"]=getDate()->toDbStringLocal();
    }
    else
    {
        ret["date"]=Json::Value();
    }
    if(getIsBackup())
    {
        ret["is_backup"]=getValueOfIsBackup();
    }
    else
    {
        ret["is_backup"]=Json::Value();
    }
    return ret;
}

Json::Value TbSign::toMasqueradedJson(
    const std::vector<std::string> &pMasqueradingVector) const
{
    Json::Value ret;
    if(pMasqueradingVector.size() == 6)
    {
        if(!pMasqueradingVector[0].empty())
        {
            if(getId())
            {
                ret[pMasqueradingVector[0]]=(Json::UInt64)getValueOfId();
            }
            else
            {
                ret[pMasqueradingVector[0]]=Json::Value();
            }
        }
        if(!pMasqueradingVector[1].empty())
        {
            if(getUserId())
            {
                ret[pMasqueradingVector[1]]=(Json::UInt64)getValueOfUserId();
            }
            else
            {
                ret[pMasqueradingVector[1]]=Json::Value();
            }
        }
        if(!pMasqueradingVector[2].empty())
        {
            if(getYear())
            {
                ret[pMasqueradingVector[2]]=getValueOfYear();
            }
            else
            {
                ret[pMasqueradingVector[2]]=Json::Value();
            }
        }
        if(!pMasqueradingVector[3].empty())
        {
            if(getMonth())
            {
                ret[pMasqueradingVector[3]]=getValueOfMonth();
            }
            else
            {
                ret[pMasqueradingVector[3]]=Json::Value();
            }
        }
        if(!pMasqueradingVector[4].empty())
        {
            if(getDate())
            {
                ret[pMasqueradingVector[4]]=getDate()->toDbStringLocal();
            }
            else
            {
                ret[pMasqueradingVector[4]]=Json::Value();
            }
        }
        if(!pMasqueradingVector[5].empty())
        {
            if(getIsBackup())
            {
                ret[pMasqueradingVector[5]]=getValueOfIsBackup();
            }
            else
            {
                ret[pMasqueradingVector[5]]=Json::Value();
            }
        }
        return ret;
    }
    LOG_ERROR << "Masquerade failed";
    if(getId())
    {
        ret["id"]=(Json::UInt64)getValueOfId();
    }
    else
    {
        ret["id"]=Json::Value();
    }
    if(getUserId())
    {
        ret["user_id"]=(Json::UInt64)getValueOfUserId();
    }
    else
    {
        ret["user_id"]=Json::Value();
    }
    if(getYear())
    {
        ret["year"]=getValueOfYear();
    }
    else
    {
        ret["year"]=Json::Value();
    }
    if(getMonth())
    {
        ret["month"]=getValueOfMonth();
    }
    else
    {
        ret["month"]=Json::Value();
    }
    if(getDate())
    {
        ret["date"]=getDate()->toDbStringLocal();
    }
    else
    {
        ret["date"]=Json::Value();
    }
    if(getIsBackup())
    {
        ret["is_backup"]=getValueOfIsBackup();
    }
    else
    {
        ret["is_backup"]=Json::Value();
    }
    return ret;
}

bool TbSign::validateJsonForCreation(const Json::Value &pJson, std::string &err)
{
    if(pJson.isMember("id"))
    {
        if(!validJsonOfField(0, "id", pJson["id"], err, true))
            return false;
    }
    if(pJson.isMember("user_id"))
    {
        if(!validJsonOfField(1, "user_id", pJson["user_id"], err, true))
            return false;
    }
    else
    {
        err="The user_id column cannot be null";
        return false;
    }
    if(pJson.isMember("year"))
    {
        if(!validJsonOfField(2, "year", pJson["year"], err, true))
            return false;
    }
    else
    {
        err="The year column cannot be null";
        return false;
    }
    if(pJson.isMember("month"))
    {
        if(!validJsonOfField(3, "month", pJson["month"], err, true))
            return false;
    }
    else
    {
        err="The month column cannot be null";
        return false;
    }
    if(pJson.isMember("date"))
    {
        if(!validJsonOfField(4, "date", pJson["date"], err, true))
            return false;
    }
    else
    {
        err="The date column cannot be null";
        return false;
    }
    if(pJson.isMember("is_backup"))
    {
        if(!validJsonOfField(5, "is_backup", pJson["is_backup"], err, true))
            return false;
    }
    return true;
}
bool TbSign::validateMasqueradedJsonForCreation(const Json::Value &pJson,
                                                const std::vector<std::string> &pMasqueradingVector,
                                                std::string &err)
{
    if(pMasqueradingVector.size() != 6)
    {
        err = "Bad masquerading vector";
        return false;
    }
    try {
      if(!pMasqueradingVector[0].empty())
      {
          if(pJson.isMember(pMasqueradingVector[0]))
          {
              if(!validJsonOfField(0, pMasqueradingVector[0], pJson[pMasqueradingVector[0]], err, true))
                  return false;
          }
      }
      if(!pMasqueradingVector[1].empty())
      {
          if(pJson.isMember(pMasqueradingVector[1]))
          {
              if(!validJsonOfField(1, pMasqueradingVector[1], pJson[pMasqueradingVector[1]], err, true))
                  return false;
          }
        else
        {
            err="The " + pMasqueradingVector[1] + " column cannot be null";
            return false;
        }
      }
      if(!pMasqueradingVector[2].empty())
      {
          if(pJson.isMember(pMasqueradingVector[2]))
          {
              if(!validJsonOfField(2, pMasqueradingVector[2], pJson[pMasqueradingVector[2]], err, true))
                  return false;
          }
        else
        {
            err="The " + pMasqueradingVector[2] + " column cannot be null";
            return false;
        }
      }
      if(!pMasqueradingVector[3].empty())
      {
          if(pJson.isMember(pMasqueradingVector[3]))
          {
              if(!validJsonOfField(3, pMasqueradingVector[3], pJson[pMasqueradingVector[3]], err, true))
                  return false;
          }
        else
        {
            err="The " + pMasqueradingVector[3] + " column cannot be null";
            return false;
        }
      }
      if(!pMasqueradingVector[4].empty())
      {
          if(pJson.isMember(pMasqueradingVector[4]))
          {
              if(!validJsonOfField(4, pMasqueradingVector[4], pJson[pMasqueradingVector[4]], err, true))
                  return false;
          }
        else
        {
            err="The " + pMasqueradingVector[4] + " column cannot be null";
            return false;
        }
      }
      if(!pMasqueradingVector[5].empty())
      {
          if(pJson.isMember(pMasqueradingVector[5]))
          {
              if(!validJsonOfField(5, pMasqueradingVector[5], pJson[pMasqueradingVector[5]], err, true))
                  return false;
          }
      }
    }
    catch(const Json::LogicError &e) 
    {
      err = e.what();
      return false;
    }
    return true;
}
bool TbSign::validateJsonForUpdate(const Json::Value &pJson, std::string &err)
{
    if(pJson.isMember("id"))
    {
        if(!validJsonOfField(0, "id", pJson["id"], err, false))
            return false;
    }
    else
    {
        err = "The value of primary key must be set in the json object for update";
        return false;
    }
    if(pJson.isMember("user_id"))
    {
        if(!validJsonOfField(1, "user_id", pJson["user_id"], err, false))
            return false;
    }
    if(pJson.isMember("year"))
    {
        if(!validJsonOfField(2, "year", pJson["year"], err, false))
            return false;
    }
    if(pJson.isMember("month"))
    {
        if(!validJsonOfField(3, "month", pJson["month"], err, false))
            return false;
    }
    if(pJson.isMember("date"))
    {
        if(!validJsonOfField(4, "date", pJson["date"], err, false))
            return false;
    }
    if(pJson.isMember("is_backup"))
    {
        if(!validJsonOfField(5, "is_backup", pJson["is_backup"], err, false))
            return false;
    }
    return true;
}
bool TbSign::validateMasqueradedJsonForUpdate(const Json::Value &pJson,
                                              const std::vector<std::string> &pMasqueradingVector,
                                              std::string &err)
{
    if(pMasqueradingVector.size() != 6)
    {
        err = "Bad masquerading vector";
        return false;
    }
    try {
      if(!pMasqueradingVector[0].empty() && pJson.isMember(pMasqueradingVector[0]))
      {
          if(!validJsonOfField(0, pMasqueradingVector[0], pJson[pMasqueradingVector[0]], err, false))
              return false;
      }
    else
    {
        err = "The value of primary key must be set in the json object for update";
        return false;
    }
      if(!pMasqueradingVector[1].empty() && pJson.isMember(pMasqueradingVector[1]))
      {
          if(!validJsonOfField(1, pMasqueradingVector[1], pJson[pMasqueradingVector[1]], err, false))
              return false;
      }
      if(!pMasqueradingVector[2].empty() && pJson.isMember(pMasqueradingVector[2]))
      {
          if(!validJsonOfField(2, pMasqueradingVector[2], pJson[pMasqueradingVector[2]], err, false))
              return false;
      }
      if(!pMasqueradingVector[3].empty() && pJson.isMember(pMasqueradingVector[3]))
      {
          if(!validJsonOfField(3, pMasqueradingVector[3], pJson[pMasqueradingVector[3]], err, false))
              return false;
      }
      if(!pMasqueradingVector[4].empty() && pJson.isMember(pMasqueradingVector[4]))
      {
          if(!validJsonOfField(4, pMasqueradingVector[4], pJson[pMasqueradingVector[4]], err, false))
              return false;
      }
      if(!pMasqueradingVector[5].empty() && pJson.isMember(pMasqueradingVector[5]))
      {
          if(!validJsonOfField(5, pMasqueradingVector[5], pJson[pMasqueradingVector[5]], err, false))
              return false;
      }
    }
    catch(const Json::LogicError &e) 
    {
      err = e.what();
      return false;
    }
    return true;
}
bool TbSign::validJsonOfField(size_t index,
                              const std::string &fieldName,
                              const Json::Value &pJson, 
                              std::string &err, 
                              bool isForCreation)
{
    switch(index)
    {
        case 0:
            if(pJson.isNull())
            {
                err="The " + fieldName + " column cannot be null";
                return false;
            }
            if(isForCreation)
            {
                err="The automatic primary key cannot be set";
                return false;
            }        
            if(!pJson.isUInt64())
            {
                err="Type error in the "+fieldName+" field";
                return false;
            }
            break;
        case 1:
            if(pJson.isNull())
            {
                err="The " + fieldName + " column cannot be null";
                return false;
            }
            if(!pJson.isUInt64())
            {
                err="Type error in the "+fieldName+" field";
                return false;
            }
            break;
        case 2:
            if(pJson.isNull())
            {
                err="The " + fieldName + " column cannot be null";
                return false;
            }
            if(!pJson.isString())
            {
                err="Type error in the "+fieldName+" field";
                return false;                
            }
            break;
        case 3:
            if(pJson.isNull())
            {
                err="The " + fieldName + " column cannot be null";
                return false;
            }
            if(!pJson.isInt())
            {
                err="Type error in the "+fieldName+" field";
                return false;
            }
            break;
        case 4:
            if(pJson.isNull())
            {
                err="The " + fieldName + " column cannot be null";
                return false;
            }
            if(!pJson.isString())
            {
                err="Type error in the "+fieldName+" field";
                return false;                
            }
            break;
        case 5:
            if(pJson.isNull())
            {
                return true;
            }
            if(!pJson.isUInt())
            {
                err="Type error in the "+fieldName+" field";
                return false;
            }
            break;
     
        default:
            err="Internal error in the server";
            return false;
            break;
    }
    return true;
}
