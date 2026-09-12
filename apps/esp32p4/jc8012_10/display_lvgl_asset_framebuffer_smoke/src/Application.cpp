#include "Application.h"

bool Application::initialize()
{
    board_.logger().info("application", "Starting JC8012 10-inch display_lvgl_asset_framebuffer_smoke");
    if (!board_.begin() || !test_.initialize())
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
