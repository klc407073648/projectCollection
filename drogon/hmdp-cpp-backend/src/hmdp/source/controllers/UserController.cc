#include <controllers/UserController.h>

UserController::UserController() : userSrvPtr_(new UserServiceImpl()) //, httpRespPtr_(new HttpResponseImpl)
{
    LOG_INFO << "UserController()";
}

UserController::~UserController()
{
    LOG_INFO << "~UserController()";
}

void UserController::userCode(const HttpRequestPtr &request, std::function<void(const HttpResponsePtr &)> &&callback)
{
    LOG_DEBUG << "UserController::userCode bgein";

    try
    {
        std::string phone = ((*(request->getJsonObject()))["phone"]).asString();
        LOG_INFO << "UserController::userCode phone: " << phone;

        if (phone.size() == 0)
        {
            throw BusinessException(ErrorCode::PARAMS_ERROR(), "入参中手机号为空");
        }

        std::string code = userSrvPtr_->sendCode(phone);

        httpRespPtr_ = HttpResponse::newHttpJsonResponse(HttpJsonResponseUtils<std::string>::makeSuccessJsonResp(code));
    }
    catch (BusinessException &e)
    {
        LOG_ERROR << "BusinessException error: message:" << e.what() << ",description" << e.getDescription();
        httpRespPtr_ = HttpResponse::newHttpJsonResponse(HttpJsonResponseUtils<BusinessException>::makeErrorJsonResp(e));
    }

    callback(httpRespPtr_);

    LOG_DEBUG << "UserController::userCode end";
}

void UserController::userLogin(const HttpRequestPtr &request, std::function<void(const HttpResponsePtr &)> &&callback)
{
    LOG_DEBUG << "UserController::userLogin bgein";

    try
    {
        std::string phone = ((*(request->getJsonObject()))["phone"]).asString();
        std::string code = ((*(request->getJsonObject()))["code"]).asString();

        LOG_INFO << "UserController::userCode phone: " << phone << ", code: " << code;

        if (phone.size() == 0 || code.size() == 0)
        {
            throw BusinessException(ErrorCode::PARAMS_ERROR(), "入参中手机号或验证码为空");
        }

        std::string token = userSrvPtr_->login(phone, code);
        httpRespPtr_ = HttpResponse::newHttpJsonResponse(HttpJsonResponseUtils<std::string>::makeSuccessJsonResp(token));
    }
    catch (BusinessException &e)
    {
        LOG_ERROR << "BusinessException error: message:" << e.what() << ",description" << e.getDescription();
        httpRespPtr_ = HttpResponse::newHttpJsonResponse(HttpJsonResponseUtils<BusinessException>::makeErrorJsonResp(e));
    }

    callback(httpRespPtr_);

    LOG_DEBUG << "UserController::userLogin end";
}
void UserController::userLogout(const HttpRequestPtr &request, std::function<void(const HttpResponsePtr &)> &&callback)
{
    LOG_DEBUG << "UserController::userLogout bgein";
    LOG_DEBUG << "UserController::userLogout end";
}
void UserController::userCurrent(const HttpRequestPtr &request, std::function<void(const HttpResponsePtr &)> &&callback)
{
    LOG_DEBUG << "UserController::userCurrent bgein";

    try
    {
        TbUser tbUser = userSrvPtr_->getCurrent(request);
        httpRespPtr_ = HttpResponse::newHttpJsonResponse(HttpJsonResponseUtils<Json::Value>::makeSuccessJsonResp(tbUser.toJson()));
    }
    catch (BusinessException &e)
    {
        LOG_ERROR << "BusinessException error: message:" << e.what() << ",description" << e.getDescription();
        httpRespPtr_ = HttpResponse::newHttpJsonResponse(HttpJsonResponseUtils<BusinessException>::makeErrorJsonResp(e));
    }

    callback(httpRespPtr_);

    LOG_DEBUG << "UserController::userCurrent end";
}
