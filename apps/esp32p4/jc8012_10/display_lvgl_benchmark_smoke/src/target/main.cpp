#include "Application.h"

extern "C" void app_main()
{
    // Static ownership keeps board peripherals and callback targets alive even
    // if initialization fails; the ESP-IDF main task may otherwise destroy them.
    static Application application;
    if (!application.initialize())
        return;
    while (true)
        application.update();
}
