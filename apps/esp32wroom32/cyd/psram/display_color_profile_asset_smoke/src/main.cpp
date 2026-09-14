#include "Application.h"

extern "C" void app_main()
{
    static cyd_color_profile_asset_smoke::Application application;
    if (!application.initialize())
        return;
    while (true)
        application.update();
}
