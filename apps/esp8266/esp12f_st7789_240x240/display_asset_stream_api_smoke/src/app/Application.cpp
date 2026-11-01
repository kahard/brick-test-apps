#include "Application.h"

namespace esp12f_st7789_asset_stream_api_smoke
{
Application::Application(const Services& services) : test_{ services.display, services.time, services.logger }
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
}  // namespace esp12f_st7789_asset_stream_api_smoke
