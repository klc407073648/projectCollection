/**
 * @file CommonUtils.h
 * @brief 正则工具类
 * @author klc
 * @date 2022-07-30
 * @copyright Copyright (c) 2022年 klc
 */

#ifndef _COMMONUTILS_H_
#define _COMMONUTILS_H_

#include <string>
#include <map>
#include <json/json.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <string>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <iostream>
#include <map>

/**
 * @brief 正则工具类
 */
class JsonUtils
{
public:
    static Json::Value ConvertMapToJson(std::map<std::string, std::string> map);
    static std::map<std::string, std::string> ConvertJsonToMap(Json::Value json);
};

/**
* @brief string工具类
*/
class StringUtil {
public:
    static std::string Format(const char* fmt, ...);
    static std::string Formatv(const char* fmt, va_list ap);

    static std::string UrlEncode(const std::string& str, bool space_as_plus = true);
    static std::string UrlDecode(const std::string& str, bool space_as_plus = true);

    static std::string Trim(const std::string& str, const std::string& delimit = " \t\r\n");
    static std::string TrimLeft(const std::string& str, const std::string& delimit = " \t\r\n");
    static std::string TrimRight(const std::string& str, const std::string& delimit = " \t\r\n");


    static std::string WStringToString(const std::wstring& ws);
    static std::wstring StringToWString(const std::string& s);
    
    static std::vector<std::string> mySplit(const std::string& str, std::string sp_string);  // split(), str 是要分割的string
    static void parseParam(std::string paramsStr, std::map<std::string, std::string> &reqParams);
    static void ClearHeadTailSpace(std::string &str);  
    static std::string getFileSuffix(std::string str);  

};

#endif //_COMMONUTILS_H_
