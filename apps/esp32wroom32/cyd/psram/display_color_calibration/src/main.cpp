#include "Application.h"

extern "C" void app_main()
{
    static cyd_color_calibration::Application application;
    if (!application.initialize())
        return;
    while (true)
        application.update();
}
