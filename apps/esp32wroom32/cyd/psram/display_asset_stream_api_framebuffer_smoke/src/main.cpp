#include "Application.h"

extern "C" void app_main()
{
    static cyd_asset_stream_smoke::Application application;
    if (!application.initialize())
        return;
    while (true)
        application.update();
}
