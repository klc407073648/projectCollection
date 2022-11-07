/**
 * @file UserDTO.h
 * @brief TbUser的数据传输对象
 * @author klc
 * @date 2022-07-30
 * @copyright Copyright (c) 2022年 klc
 */

#ifndef _USERDTO_H_
#define _USERDTO_H_

#include <string>
#include <models/TbUser.h>
#include <map>

using namespace drogon_model::hmdp;

/**
 * @brief 正则工具类
 */
class UserDTO
{
public:
    UserDTO(TbUser* tbUser);
    UserDTO(std::map<std::string,std::string> userMap);
    std::map<std::string,std::string>& getMapObject();

    //set get
    const long &getId();
    const std::string &getNickName();
    const std::string &getIcon();
    void setId(long id);
    void setNickName(const std::string &nickName);
    void setIcon(const std::string &icon);

private:
    long id;
    std::string nickName;
    std::string icon;
    std::map<std::string,std::string> userDto;
};

#endif //_USERDTO_H_
