#include <string>

#pragma once

/**
 * Redis常量类
 */
class RegexPatterns
{
public:
    /**
     * 手机号正则
    */
    static const std::string PHONE_REGEX;
    /**
     * 邮箱正则
     */
    static const std::string EMAIL_REGEX;
    /**
     * 密码正则。4~32位的字母、数字、下划线
     */
    static const std::string PASSWORD_REGEX;
    /**
     * 验证码正则, 6位数字或字母
     */
    static const std::string VERIFY_CODE_REGEX;
};

