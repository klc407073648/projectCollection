#include <controllers/VoucherOrderController.h>

VoucherOrderController::VoucherOrderController() : userSrvPtr_(new UserServiceImpl()),voucherOrderSrvPtr_(new VoucherOrderServiceImpl())//, httpRespPtr_(new HttpResponseImpl)
{
    LOG_INFO << "VoucherOrderController()";
}

VoucherOrderController::~VoucherOrderController()
{
    LOG_INFO << "~VoucherOrderController()";
}

void VoucherOrderController::seckillVoucher(const HttpRequestPtr &request, std::function<void(const HttpResponsePtr &)> &&callback,long id)
{
    LOG_DEBUG << "VoucherOrderController::seckillVoucher bgein";

    try
    {
        TbUser loginUser = userSrvPtr_->getCurrent(request);

        long res = voucherOrderSrvPtr_->seckillVoucher(id,loginUser);

        httpRespPtr_ = HttpResponse::newHttpJsonResponse(HttpJsonResponseUtils<long>::makeSuccessJsonResp(res));
    }
    catch (BusinessException &e)
    {
        LOG_ERROR << "BusinessException error: message:" << e.what() << ",description" << e.getDescription();
        httpRespPtr_ = HttpResponse::newHttpJsonResponse(HttpJsonResponseUtils<BusinessException>::makeErrorJsonResp(e));
    }

    callback(httpRespPtr_);

    LOG_DEBUG << "VoucherOrderController::seckillVoucher end";
}
