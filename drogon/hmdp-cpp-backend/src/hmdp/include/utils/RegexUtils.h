/**
 * @file RegexUtils.h
 * @brief 正则工具类
 * @author klc
 * @date 2022-07-30
 * @copyright Copyright (c) 2022年 klc
 */

#ifndef _REGEXUTILS_H_
#define _REGEXUTILS_H_

#include <string>
#include <regex>
#include <constants/RegexPatterns.h>

/**
 * @brief 正则工具类
 */
class RegexUtils
{
public:
    static bool isPhoneInvalid(const std::string &phone);
    static bool isEmailInvalid(const std::string &email);
    static bool isPasswordInvalid(const std::string &password);
    static bool isVerifyCodeInvalid(const std::string &verifyCode);
};

#endif //_REGEXUTILS_H_
