#include "Application.h"

extern "C" void app_main()
{
    static display_touch_smoke::Application application;
    if (!application.initialize())
        return;
    while (true)
        application.update();
}
