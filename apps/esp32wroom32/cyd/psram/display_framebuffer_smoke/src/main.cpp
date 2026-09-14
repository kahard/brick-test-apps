#include "Application.h"

extern "C" void app_main()
{
    static cyd_display_framebuffer_smoke::Application application;
    if (!application.initialize())
        return;
    while (true)
        application.update();
}
