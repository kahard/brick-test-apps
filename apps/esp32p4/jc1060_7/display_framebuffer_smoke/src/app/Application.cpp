#include "Application.h"

namespace display_framebuffer_smoke
{
Application::Application(const Services& services) : test_(services)
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
}  // namespace display_framebuffer_smoke
