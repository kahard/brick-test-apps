// Hardware entry point: starts the board in the configured rotation, hands its
// peripherals to the portable application and runs the loop.
#include "../app/Application.h"
#include "BoardConfig.h"
#include "brick/boards/esp32/p4/Jc8012Board.h"

namespace
{
using Board = brick::boards::Jc8012BoardTemplate<display_touch_smoke::Features>;
}  // namespace

extern "C" void app_main()
{
    using namespace display_touch_smoke;
    // Static ownership keeps board peripherals and callback targets alive even
    // if initialization fails; the ESP-IDF main task may otherwise destroy them.
    static Board board{ kRotation };
    board.logger().info("brick_jc8012_touch", "Starting JC8012 display/touch test: %ux%u rotation=%d",
                        static_cast<unsigned>(kWidth), static_cast<unsigned>(kHeight), BRICK_PANEL_ROTATION);
    if (!is_ok(board.begin()))
    {
        board.logger().error("brick_jc8012_touch", "JC8012 board initialization failed");
        return;
    }
    board.logger().info("brick_jc8012_touch", "Display and touch initialized");

    static Application application({ board.display(), board.touch(), board.memory(), board.logger() });
    if (const brick::Status status = application.initialize(); !is_ok(status))
    {
        board.logger().error("brick_jc8012_touch", "Application initialization failed: %s", to_string(status));
        return;
    }
    while (true)
    {
        application.update();
        board.time().delay_ms(1);
    }
}
