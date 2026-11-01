// Hardware entry point: starts the board, hands its peripherals to the
// portable application and runs the loop.
#include "../app/Application.h"
#include "../app/ApplicationConfig.h"
#include "brick/boards/esp32/p4/Jc1060Board.h"

namespace
{
using Board = brick::boards::Jc1060BoardTemplate<display_framebuffer_smoke::Features>;
}  // namespace

extern "C" void app_main()
{
    using namespace display_framebuffer_smoke;
    // Static ownership keeps board peripherals and callback targets alive even
    // if initialization fails; the ESP-IDF main task may otherwise destroy them.
    static Board board;
    board.logger().info("application", "Starting JC1060 7-inch display_framebuffer_smoke");
    static Application application({ board.display(), board.memory(), board.time(), board.logger() });
    if (!is_ok(board.begin()) || !is_ok(application.initialize()))
    {
        board.logger().error("application", "Initialization failed: check preceding logs, memory and assets");
        return;
    }
    board.logger().info("application", "Demo initialized");
    while (true)
    {
        application.update();
        board.time().delay_ms(1);
    }
}
