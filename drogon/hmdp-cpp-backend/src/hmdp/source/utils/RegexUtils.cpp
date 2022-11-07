#include <utils/RegexUtils.h>

bool RegexUtils::isPhoneInvalid(const std::string &phone)
{
    std::regex vaildPattern(RegexPatterns::PHONE_REGEX);
    std::smatch match;

    return regex_match(phone, match, vaildPattern);
}

bool RegexUtils::isEmailInvalid(const std::string &email)
{
    std::regex vaildPattern(RegexPatterns::EMAIL_REGEX);
    std::smatch match;

    return regex_match(email, match, vaildPattern);
}

bool RegexUtils::isPasswordInvalid(const std::string &password)
{
    std::regex vaildPattern(RegexPatterns::PASSWORD_REGEX);
    std::smatch match;

    return regex_match(password, match, vaildPattern);
}

bool RegexUtils::isVerifyCodeInvalid(const std::string &verifyCode)
{
    std::regex vaildPattern(RegexPatterns::VERIFY_CODE_REGEX);
    std::smatch match;

    return regex_match(verifyCode, match, vaildPattern);
}
