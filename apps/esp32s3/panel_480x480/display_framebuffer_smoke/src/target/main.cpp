// Hardware entry point: starts the board, hands its peripherals to the
// portable application and runs the loop.
#include "../app/Application.h"
#include "../app/ApplicationConfig.h"
#include "brick/boards/esp32/s3/Panel480Board.h"

namespace
{
using Board = brick::boards::Panel480BoardTemplate<display_framebuffer_smoke::Features>;
}  // namespace

extern "C" void app_main()
{
    using namespace display_framebuffer_smoke;
    static Board board{ Board::double_buffered_config() };
    board.logger().info("brick_st7701s_framebuffer", "Starting ESP32-S3 ST7701S framebuffer smoke test");
    if (!board.begin())
    {
        board.logger().error("brick_st7701s_framebuffer", "Board initialization failed");
        return;
    }
    static Application application({ board.display(), board.display(), board.time(), board.logger() });
    if (!application.initialize())
        return;
    while (true)
    {
        application.update();
        board.time().delay_ms(1);
    }
}
