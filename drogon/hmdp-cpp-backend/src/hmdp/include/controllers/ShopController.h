#pragma once

#include <drogon/HttpController.h>
#include <drogon/orm/Mapper.h>
#include <drogon/HttpResponse.h>
#include <services/ShopServiceImpl.h>
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

class ShopController : public drogon::HttpController<ShopController>
{
public:
  METHOD_LIST_BEGIN

  ADD_METHOD_TO(ShopController::queryShopById, "/api/shop/{id}", Get); // 根据id查询商铺信息
  ADD_METHOD_TO(ShopController::saveShop, "/api/shop", Post);   // 新增商铺信息
  ADD_METHOD_TO(ShopController::updateShop, "/api/shop", Put); // 更新商铺信息
  ADD_METHOD_TO(ShopController::queryShopByType, "/api/shop/of/type", Get);     // 根据商铺类型分页查询商铺信息
  ADD_METHOD_TO(ShopController::queryShopByName, "/api/shop/of/name", Get);     // 根据商铺名称关键字分页查询商铺信息

  METHOD_LIST_END

  /**
   * @brief 根据id查询商铺信息
   * 
   * @param request 
   * @param callback 
   */
  void queryShopById(const HttpRequestPtr &request, std::function<void(const HttpResponsePtr &)> &&callback,long shopId);

  /**
   * @brief 新增商铺信息
   * 
   * @param request 
   * @param callback 
   */
  void saveShop(const HttpRequestPtr &request, std::function<void(const HttpResponsePtr &)> &&callback);

  /**
   * @brief 更新商铺信息
   * 
   * @param request 
   * @param callback 
   */
  void updateShop(const HttpRequestPtr &request, std::function<void(const HttpResponsePtr &)> &&callback);

  /**
   * @brief 根据商铺类型分页查询商铺信息
   * 
   * @param request 
   * @param callback 
   */
  void queryShopByType(const HttpRequestPtr &request, std::function<void(const HttpResponsePtr &)> &&callback);

  /**
   * @brief 根据商铺名称关键字分页查询商铺信息
   * 
   * @param request 
   * @param callback 
   */
  void queryShopByName(const HttpRequestPtr &request, std::function<void(const HttpResponsePtr &)> &&callback);

public:
  ShopController();
  ~ShopController();

private:
   ShopServicePtr shopSrvPtr_;
   HttpResponsePtr httpRespPtr_;
};
