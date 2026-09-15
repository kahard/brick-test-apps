#include "Application.h"
namespace esp12f_st7789_asset_stream_api_smoke
{
bool Application::initialize()
{
    return board_.begin() && test_.initialize();
}
void Application::update()
{
    test_.update();
}
}  // namespace esp12f_st7789_asset_stream_api_smoke
