#include <controllers/ShopController.h>
#include <utils/CommonUtils.h>

ShopController::ShopController() : shopSrvPtr_(new ShopServiceImpl()) //, httpRespPtr_(new HttpResponseImpl)
{
    LOG_INFO << "ShopController()";
}

ShopController::~ShopController()
{
    LOG_INFO << "~ShopController()";
}

void ShopController::queryShopById(const HttpRequestPtr &request, std::function<void(const HttpResponsePtr &)> &&callback, long shopId)
{
    LOG_DEBUG << "ShopController::queryShopById bgein";

    try
    {
        TbShop tbShop = shopSrvPtr_->queryById(shopId);
        httpRespPtr_ = HttpResponse::newHttpJsonResponse(HttpJsonResponseUtils<Json::Value>::makeSuccessJsonResp(tbShop.toJson()));
    }
    catch (BusinessException &e)
    {
        LOG_ERROR << "BusinessException error: message:" << e.what() << ",description" << e.getDescription();
        httpRespPtr_ = HttpResponse::newHttpJsonResponse(HttpJsonResponseUtils<BusinessException>::makeErrorJsonResp(e));
    }

    callback(httpRespPtr_);

    LOG_DEBUG << "ShopController::queryShopById end";
}

void ShopController::saveShop(const HttpRequestPtr &request, std::function<void(const HttpResponsePtr &)> &&callback)
{
    LOG_DEBUG << "ShopController::saveShop bgein";

    try
    {
        auto json = request->getJsonObject();
        auto shop = TbShop(*json);

        LOG_INFO << "ShopController::saveShop jsonStr:" << (*json).toStyledString();

        long res = shopSrvPtr_->save(shop);
        httpRespPtr_ = HttpResponse::newHttpJsonResponse(HttpJsonResponseUtils<long>::makeSuccessJsonResp(res));
    }
    catch (BusinessException &e)
    {
        LOG_ERROR << "BusinessException error: message:" << e.what() << ",description" << e.getDescription();
        httpRespPtr_ = HttpResponse::newHttpJsonResponse(HttpJsonResponseUtils<BusinessException>::makeErrorJsonResp(e));
    }

    callback(httpRespPtr_);

    LOG_DEBUG << "ShopController::saveShop end";
}

void ShopController::updateShop(const HttpRequestPtr &request, std::function<void(const HttpResponsePtr &)> &&callback)
{
    LOG_DEBUG << "ShopController::updateShop bgein";

    try
    {
        auto json = request->getJsonObject();
        auto shop = TbShop(*json);

        LOG_INFO << "ShopController::updateShop jsonStr:" << (*json).toStyledString();

        long res = shopSrvPtr_->update(shop);
        httpRespPtr_ = HttpResponse::newHttpJsonResponse(HttpJsonResponseUtils<long>::makeSuccessJsonResp(res));
    }
    catch (BusinessException &e)
    {
        LOG_ERROR << "BusinessException error: message:" << e.what() << ",description" << e.getDescription();
        httpRespPtr_ = HttpResponse::newHttpJsonResponse(HttpJsonResponseUtils<BusinessException>::makeErrorJsonResp(e));
    }

    callback(httpRespPtr_);

    LOG_DEBUG << "ShopController::updateShop end";
}

void ShopController::queryShopByName(const HttpRequestPtr &request, std::function<void(const HttpResponsePtr &)> &&callback)
{
    LOG_DEBUG << "ShopController::queryShopByName bgein";
    LOG_DEBUG << "ShopController::queryShopByName end";
}

void ShopController::queryShopByType(const HttpRequestPtr &request, std::function<void(const HttpResponsePtr &)> &&callback)
{
    LOG_DEBUG << "ShopController::queryShopByType bgein";

    try
    {
        auto queryStr = request->query();

		//queryStr:typeId=1&current=1&x=120.149993&y=30.334229 
        LOG_INFO << "ShopController::queryShopByType queryStr:" << queryStr;
		
		std::map<std::string, std::string> map;
		StringUtil::parseParam(queryStr, map);
		Json::Value json = JsonUtils::ConvertMapToJson(map);
		
        /*
        {
            "current" : "1",
            "typeId" : "1",
            "x" : "120.149993",
            "y" : "30.334229"
        }
        */
        //TODO  这个输入的数据为string，转换成int,double会报错，还需要处理
		LOG_INFO << "ShopController::queryShopByType jsonStr:" << json.toStyledString();
        std::vector<TbShop> shops = shopSrvPtr_->queryByType(json);

        httpRespPtr_ = HttpResponse::newHttpJsonResponse(HttpJsonResponseUtils<std::vector<TbShop>>::makeSuccessJsonResp(shops));
    }
    catch (BusinessException &e)
    {
        LOG_ERROR << "BusinessException error: message:" << e.what() << ",description" << e.getDescription();
        httpRespPtr_ = HttpResponse::newHttpJsonResponse(HttpJsonResponseUtils<BusinessException>::makeErrorJsonResp(e));
    }

    callback(httpRespPtr_);

    LOG_DEBUG << "ShopController::queryShopByType end";
}
