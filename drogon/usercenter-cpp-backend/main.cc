#include <drogon/drogon.h>

using namespace drogon;

int main()
{
    //app().addListener("0.0.0.0", 8082).run();
	app().loadConfigFile("../config.json").run();
}
