#include <drogon/drogon.h>
#include <common/Resource.h>
#include <common/GlobalExceptionHandler.h>

using namespace drogon;

int main()
{
    Resource res;
    res.loadConfigFile("/home/backend/src/conf/config.yml");
    //app().addListener("0.0.0.0", 8082).run();
	app().setExceptionHandler(GlobalExceptionHandler::ExceptionHandler);
	app().loadConfigFile("/home/backend/src/conf/config.json").run();
}
