#include "Application.h"

namespace asset_stream_smoke
{
Application::Application(const Services& services)
    : sources_(services.flash_assets, services.sd, services.memory),
      presenter_(services.display, services.frame_buffers), playback_(sources_, presenter_),
      test_(playback_, services.touch, services.time, services.logger)
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
}  // namespace asset_stream_smoke
