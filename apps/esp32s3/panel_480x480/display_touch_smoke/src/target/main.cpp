// Hardware entry point: starts the board, hands its peripherals to the
// portable application and runs the loop.
#include "../app/Application.h"
#include "../app/ApplicationConfig.h"
#include "brick/boards/esp32/s3/Panel480Board.h"

namespace
{
using Board = brick::boards::Panel480BoardTemplate<display_touch_smoke::DisplayTouchFeatures>;
}  // namespace

extern "C" void app_main()
{
    using namespace display_touch_smoke;
    static Board board;
    board.logger().info("brick_st7701s_smoke", "Starting ESP32-S3 ST7701S/GT911 smoke test");
    if (!board.begin())
    {
        board.logger().error("brick_st7701s_smoke", "ST7701S RGB display initialization failed");
        return;
    }
    board.logger().info("brick_st7701s_smoke", "Display and touch initialized");

    static Application application({ board.display(), board.touch(), board.time(), board.logger() });
    if (!application.initialize())
        return;
    while (true)
    {
        application.update();
        board.time().delay_ms(1);
    }
}
