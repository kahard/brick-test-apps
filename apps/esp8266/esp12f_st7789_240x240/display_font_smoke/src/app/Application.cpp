#include "Application.h"

namespace esp12f_st7789_display_font_smoke
{
Application::Application(const Services& services) : test_{ services.display, services.button, services.logger }
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
}  // namespace esp12f_st7789_display_font_smoke
