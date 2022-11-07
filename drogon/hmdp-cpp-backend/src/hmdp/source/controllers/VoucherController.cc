#include <controllers/VoucherController.h>

VoucherController::VoucherController() : voucherSrvPtr_(new VoucherServiceImpl()) //, httpRespPtr_(new HttpResponseImpl)
{
    LOG_INFO << "VoucherController()";
}

VoucherController::~VoucherController()
{
    LOG_INFO << "~VoucherController()";
}

void VoucherController::addSeckillVoucher(const HttpRequestPtr &request, std::function<void(const HttpResponsePtr &)> &&callback)
{
    LOG_DEBUG << "VoucherController::addSeckillVoucher bgein";

    try
    {
        //通过POST的JSON请求来添加秒杀券，且不需要携带id信息，通过voucherMapper.insert时，会根据AUTO_INCREMENT属性自动填写
        //Stock,BeginTime,EndTime非数据库表 TbVoucher 中字段，在POST请求中加上
        auto json = request->getJsonObject();
        auto voucher = TbVoucher(*json);

        LOG_INFO << "VoucherController::addSeckillVoucher jsonStr:" << (*json).toStyledString();

        voucherSrvPtr_->addSeckillVoucher(voucher,*json);

        httpRespPtr_ = HttpResponse::newHttpJsonResponse(HttpJsonResponseUtils<long>::makeSuccessJsonResp(voucher.getValueOfId()));
    }
    catch (BusinessException &e)
    {
        LOG_ERROR << "BusinessException error: message:" << e.what() << ",description" << e.getDescription();
        httpRespPtr_ = HttpResponse::newHttpJsonResponse(HttpJsonResponseUtils<BusinessException>::makeErrorJsonResp(e));
    }

    callback(httpRespPtr_);

    LOG_DEBUG << "VoucherController::addSeckillVoucher end";
}

void VoucherController::addVoucher(const HttpRequestPtr &request, std::function<void(const HttpResponsePtr &)> &&callback)
{
    LOG_DEBUG << "VoucherController::addVoucher bgein";

    try
    {
        auto json = request->getJsonObject();
        auto voucher = TbVoucher(*json);

        LOG_INFO << "VoucherController::addVoucher jsonStr:" << (*json).toStyledString();

        voucherSrvPtr_->addVoucher(voucher,*json);

        httpRespPtr_ = HttpResponse::newHttpJsonResponse(HttpJsonResponseUtils<long>::makeSuccessJsonResp(voucher.getValueOfId()));
    }
    catch (BusinessException &e)
    {
        LOG_ERROR << "BusinessException error: message:" << e.what() << ",description" << e.getDescription();
        httpRespPtr_ = HttpResponse::newHttpJsonResponse(HttpJsonResponseUtils<BusinessException>::makeErrorJsonResp(e));
    }

    callback(httpRespPtr_);

    LOG_DEBUG << "VoucherController::addVoucher end";
}

void VoucherController::queryVoucherOfShop(const HttpRequestPtr &request, std::function<void(const HttpResponsePtr &)> &&callback, long shopId)
{
    LOG_DEBUG << "VoucherController::queryVoucherOfShop bgein";

    try
    {
        std::vector<TbVoucher> vouchers = voucherSrvPtr_->queryVoucherOfShop(shopId);
        httpRespPtr_ = HttpResponse::newHttpJsonResponse(HttpJsonResponseUtils<std::vector<TbVoucher>>::makeSuccessJsonResp(vouchers));
    }
    catch (BusinessException &e)
    {
        LOG_ERROR << "BusinessException error: message:" << e.what() << ",description" << e.getDescription();
        httpRespPtr_ = HttpResponse::newHttpJsonResponse(HttpJsonResponseUtils<BusinessException>::makeErrorJsonResp(e));
    }

    callback(httpRespPtr_);

    LOG_DEBUG << "VoucherController::queryVoucherOfShop end";
}
