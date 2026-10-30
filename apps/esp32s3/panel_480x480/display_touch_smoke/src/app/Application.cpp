#include "Application.h"

namespace display_touch_smoke
{
bool Application::initialize()
{
    board_.logger().info("brick_st7701s_smoke", "Starting ESP32-S3 ST7701S/GT911 smoke test");
    if (!board_.begin())
    {
        board_.logger().error("brick_st7701s_smoke", "ST7701S RGB display initialization failed");
        return false;
    }
    board_.logger().info("brick_st7701s_smoke", "Display and touch initialized");
    return test_.initialize();
}

void Application::update()
{
    test_.update();
    board_.time().delay_ms(1);
}
}  // namespace display_touch_smoke
