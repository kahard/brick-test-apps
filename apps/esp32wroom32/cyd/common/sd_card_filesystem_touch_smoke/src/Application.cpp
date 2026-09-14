#include "Application.h"

namespace cyd_sd_card_filesystem_touch_smoke
{
bool Application::initialize()
{
    return board_.begin() && test_.initialize();
}

void Application::update()
{
    test_.update();
}
}  // namespace cyd_sd_card_filesystem_touch_smoke
