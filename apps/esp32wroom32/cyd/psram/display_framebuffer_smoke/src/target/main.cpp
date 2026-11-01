// CYD entry point: builds the board, hands its peripherals to the portable
// application and runs the loop.
#include "../app/Application.h"
#include "BoardFeatures.h"
#include "brick/boards/esp32/wroom32/CydBoard.h"

namespace
{
using Board = brick::boards::CydBoardTemplate<cyd_display_framebuffer_smoke::FramebufferFeatures>;
}

extern "C" void app_main()
{
    static Board board;
    if (!is_ok(board.begin()))
    {
        board.logger().error("brick_cyd_framebuffer", "CYD display initialization failed");
        return;
    }

    static cyd_display_framebuffer_smoke::Application application(
        { board.display(), board.memory(), board.time(), board.logger() });
    if (const brick::Status status = application.initialize(); !is_ok(status))
    {
        board.logger().error("brick_cyd_framebuffer", "Application initialization failed: %s", to_string(status));
        return;
    }
    while (true)
    {
        application.update();
        board.time().delay_ms(1);
    }
}
