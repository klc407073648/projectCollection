#pragma once

#include <drogon/HttpController.h>
#include <drogon/orm/Mapper.h>
#include <drogon/HttpResponse.h>
#include <services/VoucherServiceImpl.h>
#include <models/TbVoucher.h>
#include <models/TbSeckillVoucher.h>
#include <common/ResultUtils.h>
#include <common/Response2json.h>
#include <common/HttpJsonResponseUtils.h>
#include <string>
#include <json/json.h>

using namespace drogon;
using namespace drogon_model::hmdp;
using namespace drogon::orm;
using namespace hmdp;

class VoucherController : public drogon::HttpController<VoucherController>
{
public:
  METHOD_LIST_BEGIN

  ADD_METHOD_TO(VoucherController::addSeckillVoucher, "/api/voucher/seckill", Post); // 新增秒杀券
  ADD_METHOD_TO(VoucherController::addVoucher, "/api/voucher", Post);   // 新增普通券
  ADD_METHOD_TO(VoucherController::queryVoucherOfShop, "/api/voucher/list/{shopId}", Get); // 查询店铺的优惠券列表

  METHOD_LIST_END

  /**
   * @brief 新增秒杀券
   * 
   * @param request 
   * @param callback 
   */
  void addSeckillVoucher(const HttpRequestPtr &request, std::function<void(const HttpResponsePtr &)> &&callback);

  /**
   * @brief 新增普通券
   * 
   * @param request 
   * @param callback 
   */
  void addVoucher(const HttpRequestPtr &request, std::function<void(const HttpResponsePtr &)> &&callback);

  /**
   * @brief 查询店铺的优惠券列表
   * 
   * @param request 
   * @param callback 
   */
  void queryVoucherOfShop(const HttpRequestPtr &request, std::function<void(const HttpResponsePtr &)> &&callback,long shopId);

public:
  VoucherController();
  ~VoucherController();

private:
   VoucherServicePtr voucherSrvPtr_;
   HttpResponsePtr httpRespPtr_;
};
