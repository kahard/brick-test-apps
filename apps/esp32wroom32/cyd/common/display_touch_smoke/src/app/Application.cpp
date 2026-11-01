#include "Application.h"

namespace cyd_display_touch_smoke
{
Application::Application(const Services& services)
    : services_(services), test_{ services.display, services.touch, services.time, services.logger }
{
}

Status Application::initialize()
{
    services_.logger.info("brick_cyd_touch", "Starting CYD display and touch smoke test");
    return test_.initialize();
}

void Application::update()
{
    test_.update();
}
}  // namespace cyd_display_touch_smoke
