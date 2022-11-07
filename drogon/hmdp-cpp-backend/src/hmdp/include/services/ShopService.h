#pragma once

#include <memory>
#include <string>
#include <vector>
#include <models/TbShop.h>
#include <drogon/HttpController.h>
#include <dto/UserDTO.h>

using namespace drogon;
using namespace drogon_model::hmdp;

// ShopService 纯虚函数，接口类
namespace hmdp
{

  class ShopService;
  using ShopServicePtr = std::shared_ptr<ShopService>;

  enum CacheType
  {
    DEFAULT = 0,     //默认
    CHCHE_PENETRATE, //缓存穿透
    CHCHE_AVALANCHE, //缓存雪崩
    CHCHE_BREAKDOWN  //缓存击穿
  };
  
  class ShopService
  {
  public:
    /**
     * @brief 根据id查询商铺
     *
     * @param id
     * @return std::string
     */
    virtual TbShop queryById(long id, CacheType type = DEFAULT) = 0;

    /**
     * @brief 新增商铺信息
     *
     * @param tbShop
     * @return long
     */
    virtual long save(const TbShop &tbShop) = 0;

    /**
     * @brief 更新商铺信息
     *
     * @param tbShop
     * @return long
     */
    virtual long update(const TbShop &tbShop) = 0;

    /**
     * @brief 根据商铺类型查询商铺信息
     *
     * @param request
     * @return TbShop
     */
    virtual std::vector<TbShop> queryByType(Json::Value jsonObj) = 0;
  };
}
