#include "Application.h"

extern "C" void app_main()
{
    Application application;
    if (!application.initialize())
        return;
    while (true)
        application.update();
}
