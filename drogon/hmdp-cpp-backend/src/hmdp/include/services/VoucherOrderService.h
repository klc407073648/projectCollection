#pragma once

#include <memory>
#include <string>
#include <vector>
#include <models/TbSeckillVoucher.h>
#include <models/TbVoucherOrder.h>
#include <drogon/HttpController.h>
#include <dto/UserDTO.h>

using namespace drogon;
using namespace drogon_model::hmdp;

// VoucherOrderService 纯虚函数，接口类
namespace hmdp
{

  class VoucherOrderService;
  using VoucherOrderServicePtr = std::shared_ptr<VoucherOrderService>;

  class VoucherOrderService
  {
  public:
    /**
     * @brief 秒杀优惠券
     *
     * @param voucherId   秒杀优惠券
     * @return voucherId
     */
    virtual long seckillVoucher(long voucherId,TbUser loginUser) = 0;
  };
}
