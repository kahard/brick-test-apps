#include "Application.h"

namespace cyd_lvgl_asset_smoke
{
Application::Application(const Services& services) : demo_(services)
{
}

Status Application::initialize()
{
    return demo_.initialize();
}

void Application::update()
{
    demo_.update();
}
}  // namespace cyd_lvgl_asset_smoke
