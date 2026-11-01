// Hardware entry point: starts the board, hands its peripherals to the
// portable application and runs the loop.
#include "../app/Application.h"
#include "../app/ApplicationConfig.h"
#include "brick/boards/esp32/s3/Panel480Board.h"

namespace
{
using Board = brick::boards::Panel480BoardTemplate<SdFeatures>;
}  // namespace

extern "C" void app_main()
{
    static Board board;
    if (!board.begin())
    {
        board.logger().error("panel480_sd_card_filesystem_touch_smoke", "board initialization failed");
        return;
    }
    static Application application({ board.display(), board.touch(), board.sd(), board.time(), board.logger() });
    if (!application.initialize())
        return;
    // The application's update() does not block; the loop paces it.
    while (true)
    {
        application.update();
        board.time().delay_ms(30);
    }
}
