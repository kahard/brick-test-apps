#include "Application.h"

namespace display_touch_smoke
{
bool Application::initialize()
{
    board_.logger().info("brick_jc8012_touch", "Starting JC8012 display/touch test: %ux%u rotation=%d",
                         static_cast<unsigned>(kWidth), static_cast<unsigned>(kHeight), BRICK_PANEL_ROTATION);
    if (!board_.begin())
    {
        board_.logger().error("brick_jc8012_touch", "JC8012 board initialization failed");
        return false;
    }
    board_.logger().info("brick_jc8012_touch", "Display and touch initialized");
    return test_.initialize();
}

void Application::update()
{
    test_.update();
    board_.time().delay_ms(1);
}
}  // namespace display_touch_smoke
