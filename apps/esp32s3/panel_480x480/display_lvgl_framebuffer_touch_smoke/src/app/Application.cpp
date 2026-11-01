#include "Application.h"

namespace lvgl_framebuffer_touch_smoke
{
Application::Application(const Services& services) : test_(services)
{
}

Status Application::initialize()
{
    return test_.initialize();
}

void Application::update()
{
    test_.update();
}
}  // namespace lvgl_framebuffer_touch_smoke
