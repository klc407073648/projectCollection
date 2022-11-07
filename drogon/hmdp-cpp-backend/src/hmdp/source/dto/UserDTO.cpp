#include <dto/UserDTO.h>

UserDTO::UserDTO(TbUser *tbUser)
{
    id = tbUser->getValueOfId();
    nickName = tbUser->getValueOfNickName();
    icon = tbUser->getValueOfIcon();

    userDto["id"] = std::to_string(id);
    userDto["nickName"] = nickName;
    userDto["icon"] = icon;
}

UserDTO::UserDTO(std::map<std::string, std::string> userMap)
{
    id = stol(userMap["id"]);
    nickName = userMap["nickName"];
    icon = userMap["icon"];

    userDto["id"] = id;
    userDto["nickName"] = nickName;
    userDto["icon"] = icon;
}

std::map<std::string, std::string> &UserDTO::getMapObject()
{
    return userDto;
}

const long &UserDTO::getId()
{
    return id;
}

const std::string &UserDTO::getNickName()
{
    return nickName;
}

const std::string &UserDTO::getIcon()
{
    return icon;
}

void UserDTO::setId(long id)
{
    this->id = id;
}

void UserDTO::setNickName(const std::string &nickName)
{
    this->nickName = nickName;
}

void UserDTO::setIcon(const std::string &icon)
{
    this->icon = icon;
}
