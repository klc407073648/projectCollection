#include <string>

#pragma once
/**
 * Redis常量类
 */
class SystemConstants
{
public:
    static const std::string IMAGE_UPLOAD_DIR;
    static const std::string USER_NICK_NAME_PREFIX;
    static const int DEFAULT_PAGE_SIZE = 5;
    static const int MAX_PAGE_SIZE = 10;
};