#include "Application.h"

namespace cyd_asset_stream_smoke
{
bool Application::initialize()
{
    return board_.begin() && test_.initialize();
}

void Application::update()
{
    test_.update();
}
}  // namespace cyd_asset_stream_smoke
