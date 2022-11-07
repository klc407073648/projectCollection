/**
 * @file RandomUtils.h
 * @brief 随机数工具类
 * @author klc
 * @date 2022-07-30
 * @copyright Copyright (c) 2022年 klc
 */

#ifndef _RANDOMUTILS_H_
#define _RANDOMUTILS_H_

#include <string>
#include <math.h>
#include <iostream>
#include <ctime>
#include <vector>
#include <sstream>
#include <iterator>

/**
 * @brief 随机数工具类
 */
class RandomUtils
{
public:
    static std::string getRandomNumberStr(int n);
};

#endif //_RANDOMUTILS_H_
