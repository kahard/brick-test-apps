#include "Application.h"

extern "C" void app_main()
{
    static cyd_lvgl_asset_smoke::Application application;
    if (!application.initialize())
        return;
    while (true)
        application.update();
}
