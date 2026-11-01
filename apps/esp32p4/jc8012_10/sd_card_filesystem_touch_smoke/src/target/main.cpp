// Hardware entry point: starts the board, hands its peripherals to the
// portable application and runs the loop.
#include "../app/Application.h"
#include "../app/ApplicationConfig.h"
#include "brick/boards/esp32/p4/Jc8012Board.h"

namespace
{
using Board = brick::boards::Jc8012BoardTemplate<Features>;
}  // namespace

extern "C" void app_main()
{
    static Board board;
    if (!is_ok(board.begin()))
    {
        board.logger().error("jc8012_sd_card_filesystem_touch_smoke", "Display/touch initialization failed");
        return;
    }
    static Application application({ board.display(), board.touch(), board.sd(), board.time(), board.logger() });
    if (const brick::Status status = application.initialize(); !is_ok(status))
    {
        board.logger().error("jc8012_sd_card_filesystem_touch_smoke", "Application initialization failed: %s",
                             to_string(status));
        return;
    }
    // The application's update() does not block; the loop paces it.
    while (true)
    {
        application.update();
        board.time().delay_ms(30);
    }
}
