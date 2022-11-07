#pragma once

#include <drogon/HttpController.h>
#include <drogon/orm/Mapper.h>
#include <drogon/HttpResponse.h>
#include <services/VoucherOrderServiceImpl.h>
#include <services/UserServiceImpl.h>
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

class VoucherOrderController : public drogon::HttpController<VoucherOrderController>
{
public:
  METHOD_LIST_BEGIN

  ADD_METHOD_TO(VoucherOrderController::seckillVoucher, "/api/voucher-order/seckill/{id}", Post); // 秒杀优惠券

  METHOD_LIST_END

  /**
   * @brief 秒杀优惠券
   * 
   * @param request 
   * @param callback 
   */
  void seckillVoucher(const HttpRequestPtr &request, std::function<void(const HttpResponsePtr &)> &&callback,long id);

public:
  VoucherOrderController();
  ~VoucherOrderController();
  

private:
   VoucherOrderServicePtr voucherOrderSrvPtr_;
   UserServicePtr userSrvPtr_;
   HttpResponsePtr httpRespPtr_;
};
