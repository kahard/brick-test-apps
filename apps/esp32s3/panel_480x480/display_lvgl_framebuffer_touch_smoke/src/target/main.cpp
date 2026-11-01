// Hardware entry point: starts the board with two scan-out buffers, hands its
// peripherals to the portable application and runs the loop.
#include "../app/Application.h"
#include "../app/ApplicationConfig.h"
#include "brick/boards/esp32/s3/Panel480Board.h"

namespace
{
using Board = brick::boards::Panel480BoardTemplate<lvgl_framebuffer_touch_smoke::LvglFeatures>;
}  // namespace

extern "C" void app_main()
{
    using namespace lvgl_framebuffer_touch_smoke;
    static Board board{ Board::double_buffered_config() };
    board.logger().info("brick_st7701s_lvgl_fb", "Starting ESP32-S3 ST7701S LVGL framebuffer + GT911 smoke test");
    if (!is_ok(board.begin()))
    {
        board.logger().error("brick_st7701s_lvgl_fb", "Board initialization failed");
        return;
    }
    static Application application({ board.display(), board.display(), board.touch(), board.time(), board.logger() });
    if (const brick::Status status = application.initialize(); !is_ok(status))
    {
        board.logger().error("brick_st7701s_lvgl_fb", "Application initialization failed: %s", to_string(status));
        return;
    }
    while (true)
    {
        application.update();
        board.time().delay_ms(1);
    }
}
