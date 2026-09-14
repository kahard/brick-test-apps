#include "Application.h"

namespace cyd_lvgl_asset_smoke
{
bool Application::initialize()
{
    return board_.begin() && demo_.initialize();
}

void Application::update()
{
    demo_.update();
}
}  // namespace cyd_lvgl_asset_smoke
