#pragma once

#include <memory>
#include <string>
#include <vector>
#include <models/TbUser.h>
#include <drogon/HttpController.h>
#include <dto/UserDTO.h>

using namespace drogon;
using namespace drogon_model::hmdp;

// UserService 纯虚函数，接口类
namespace hmdp
{

  class UserService;
  using UserServicePtr = std::shared_ptr<UserService>;

  class UserService
  {
  public:
    /**
     * @brief 发送手机验证码
     *
     * @param userAccount   手机号
     * @param request       原始请求
     * @return 新用户 id
     */
    virtual std::string sendCode(const std::string &phone) = 0;

    /**
     * @brief 用户登陆
     *
     * @param request      原始请求
     * @return 脱敏后的用户信息
     */
    virtual std::string login(const std::string &phone,const std::string &code) = 0;

    /**
     * @brief 用户注销
     *
     * @param request  原始请求
     * @return 注销是否成功
     */
    virtual long logout(const HttpRequestPtr &request) = 0;

    /**
     * @brief 获取当前用户信息
     *
     * @param id    用户ID
     * @return 当前用户
     */
    virtual TbUser getCurrent(const HttpRequestPtr &request) = 0;

  };
}
