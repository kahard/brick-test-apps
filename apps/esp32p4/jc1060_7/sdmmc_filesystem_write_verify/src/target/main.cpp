// Hardware entry point: starts the board, hands its peripherals to the
// portable application and runs the loop.
#include "../app/Application.h"
#include "../app/ApplicationConfig.h"
#include "brick/boards/esp32/p4/Jc1060Board.h"

namespace
{
using Board = brick::boards::Jc1060BoardTemplate<SdFeatures>;
}  // namespace

extern "C" void app_main()
{
    static Board board;
    if (!is_ok(board.begin()))
    {
        board.logger().error("jc1060_sdmmc_filesystem_write_verify", "Display/touch initialization failed");
        return;
    }
    static Application application({ board.display(), board.touch(), board.sd(), board.time(), board.logger() });
    if (const brick::Status status = application.initialize(); !is_ok(status))
    {
        board.logger().error("jc1060_sdmmc_filesystem_write_verify", "Application initialization failed: %s",
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
