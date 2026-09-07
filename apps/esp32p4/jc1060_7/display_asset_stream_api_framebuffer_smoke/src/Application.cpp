#include "Application.h"

bool Application::initialize()
{
    board_.logger().info("application", "Starting JC1060 7-inch display_asset_stream_api_framebuffer_smoke");
    if (!board_.begin() || !sources_.initialize() || !test_.initialize())
    {
        board_.logger().error("application", "Initialization failed: check preceding logs, memory and assets");
        return false;
    }
    board_.logger().info("application", "Demo initialized");
    return true;
}
void Application::update()
{
    test_.update();
    board_.time().delay_ms(1);
}
