#pragma once

#include <memory>
#include <string>
#include <vector>
#include <models/TbVoucher.h>
#include <drogon/HttpController.h>
#include <dto/UserDTO.h>
#include <json/json.h>

using namespace drogon;
using namespace drogon_model::hmdp;

// VoucherService 纯虚函数，接口类
namespace hmdp
{

  class VoucherService;
  using VoucherServicePtr = std::shared_ptr<VoucherService>;

  class VoucherService
  {
  public:
    /**
     * @brief 新增秒杀券
     *
     * @param voucher   秒杀券信息
     * @return voucher id
     */
    virtual void addSeckillVoucher(TbVoucher &voucher,Json::Value jsonObj) = 0;

    /**
     * @brief 新增普通券
     *
     * @param voucher   普通券信息
     * @return voucher id
     */
    virtual void addVoucher(TbVoucher &voucher,Json::Value jsonObj)  = 0;

    /**
     * @brief 查询店铺的优惠券列表
     *
     * @param shopId      商铺id
     * @return voucher id
     */
    virtual std::vector<TbVoucher>  queryVoucherOfShop(long shopId) = 0;

  };
}
