#include "Application.h"

extern "C" void app_main()
{
    static cyd_sd_card_filesystem_touch_smoke::Application application;
    if (!application.initialize())
        return;
    while (true)
        application.update();
}
