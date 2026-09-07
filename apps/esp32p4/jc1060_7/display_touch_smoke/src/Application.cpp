#include "Application.h"

namespace display_touch_smoke
{
bool Application::initialize()
{
    board_.logger().info("brick_display_touch_smoke", "Starting JC1060 display/touch smoke test");
    if (!board_.begin())
    {
        board_.logger().error("brick_display_touch_smoke", "JC1060 board initialization failed");
        return false;
    }
    board_.logger().info("brick_display_touch_smoke", "Display and touch initialized");
    board_.time().delay_ms(2000);
    return test_.initialize();
}

void Application::update()
{
    test_.update();
    board_.time().delay_ms(1);
}
}  // namespace display_touch_smoke
