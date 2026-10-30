#include "Application.h"

namespace display_touch_smoke
{
Application::Application(const Services& services) : test_{ services.display, services.touch, services.time, services.logger }
{
}

bool Application::initialize()
{
    return test_.initialize();
}

void Application::update()
{
    test_.update();
}
}  // namespace display_touch_smoke
