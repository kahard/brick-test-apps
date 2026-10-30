// Hardware entry point: starts the board, hands its peripherals to the
// portable application and runs the loop.
#include "../app/Application.h"
#include "../app/ApplicationConfig.h"
#include "brick/boards/esp32/p4/Jc1060Board.h"

namespace
{
using Board = brick::boards::Jc1060BoardTemplate<display_touch_smoke::Features>;
}  // namespace

extern "C" void app_main()
{
    using namespace display_touch_smoke;
    static Board board;
    board.logger().info("brick_display_touch_smoke", "Starting JC1060 display/touch smoke test");
    if (!board.begin())
    {
        board.logger().error("brick_display_touch_smoke", "JC1060 board initialization failed");
        return;
    }
    board.logger().info("brick_display_touch_smoke", "Display and touch initialized");
    board.time().delay_ms(2000);

    static Application application({ board.display(), board.touch(), board.time(), board.logger() });
    if (!application.initialize())
        return;
    while (true)
    {
        application.update();
        board.time().delay_ms(1);
    }
}
