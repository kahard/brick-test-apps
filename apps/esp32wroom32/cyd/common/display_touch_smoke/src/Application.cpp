#include "Application.h"

namespace cyd_display_touch_smoke
{
bool Application::initialize()
{
    board_.logger().info("brick_cyd_touch", "Starting CYD display and touch smoke test");
    if (!board_.begin())
    {
        board_.logger().error("brick_cyd_touch", "CYD display or touch initialization failed");
        return false;
    }
    board_.logger().info("brick_cyd_touch", "CYD display and touch initialized");
    return test_.initialize();
}

void Application::update()
{
    test_.update();
    board_.time().delay_ms(1);
}
}  // namespace cyd_display_touch_smoke
