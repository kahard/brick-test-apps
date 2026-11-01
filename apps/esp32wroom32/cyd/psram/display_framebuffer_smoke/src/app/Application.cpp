#include "Application.h"

namespace cyd_display_framebuffer_smoke
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

}  // namespace cyd_display_framebuffer_smoke
