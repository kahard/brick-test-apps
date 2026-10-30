#include "Application.h"

namespace cyd_sd_card_filesystem_touch_smoke
{
Application::Application(const Services& services) : test_{ services.display, services.sd, services.touch, services.time, services.logger }
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
}  // namespace cyd_sd_card_filesystem_touch_smoke
