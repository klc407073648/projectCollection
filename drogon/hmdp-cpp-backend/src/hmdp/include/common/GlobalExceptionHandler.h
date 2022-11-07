#pragma once

#include <drogon/HttpController.h>
#include <drogon/HttpResponse.h>
#include <common/ResultUtils.h>
#include <common/Response2json.h>
#include <common/HttpJsonResponseUtils.h>

using namespace drogon;

/**
 * 全局异常处理类
 */

// TODO 目前只能捕获 std::exception，用于兜底其他异常，业务层面的异常在controller里捕获
class GlobalExceptionHandler : public std::exception
{
public:
    static void ExceptionHandler(const std::exception &e, const HttpRequestPtr &httpReqPtr, std::function<void(const HttpResponsePtr &)> &&callback)
    {
        LOG_ERROR <<  "GlobalExceptionHandler: " << e.what();
        BusinessException globalException = BusinessException(ErrorCode::GLOBAL_ERROR(), e.what());
        HttpResponsePtr httpRespPtr = HttpResponse::newHttpJsonResponse(HttpJsonResponseUtils<BusinessException>::makeErrorJsonResp(globalException));
        callback(httpRespPtr);
    }
};