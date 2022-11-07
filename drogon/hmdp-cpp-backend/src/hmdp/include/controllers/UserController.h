#pragma once

#include <drogon/HttpController.h>
#include <drogon/orm/Mapper.h>
#include <drogon/HttpResponse.h>
#include <services/UserServiceImpl.h>
#include <models/TbUser.h>
#include <common/ResultUtils.h>
#include <common/Response2json.h>
#include <common/HttpJsonResponseUtils.h>
#include <string>
#include <json/json.h>
#include <dto/UserDTO.h>

using namespace drogon;
using namespace drogon_model::hmdp;
using namespace drogon::orm;
using namespace hmdp;

class UserController : public drogon::HttpController<UserController>
{
public:
  METHOD_LIST_BEGIN

  ADD_METHOD_TO(UserController::userCode, "/api/user/code", Post); // 发送手机验证码
  ADD_METHOD_TO(UserController::userLogin, "/api/user/login", Post);   // 登录功能
  ADD_METHOD_TO(UserController::userLogout, "/api/user/logout", Post); // 登出功能
  ADD_METHOD_TO(UserController::userCurrent, "/api/user/me", Get);     // 获取当前用户

  METHOD_LIST_END

  /**
   * @brief 发送手机验证码
   * 
   * @param request 
   * @param callback 
   */
  void userCode(const HttpRequestPtr &request, std::function<void(const HttpResponsePtr &)> &&callback);

  /**
   * @brief 登录功能
   * 
   * @param request 
   * @param callback 
   */
  void userLogin(const HttpRequestPtr &request, std::function<void(const HttpResponsePtr &)> &&callback);

  /**
   * @brief 登出功能
   * 
   * @param request 
   * @param callback 
   */
  void userLogout(const HttpRequestPtr &request, std::function<void(const HttpResponsePtr &)> &&callback);

  /**
   * @brief 获取当前用户
   * 
   * @param request 
   * @param callback 
   */
  void userCurrent(const HttpRequestPtr &request, std::function<void(const HttpResponsePtr &)> &&callback);

public:
  UserController();
  ~UserController();

private:
  UserServicePtr userSrvPtr_;
  HttpResponsePtr httpRespPtr_;
};
