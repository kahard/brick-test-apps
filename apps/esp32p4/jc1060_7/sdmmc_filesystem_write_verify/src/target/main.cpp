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
    if (!board.begin())
    {
        board.logger().error("jc1060_sdmmc_filesystem_write_verify", "Display/touch initialization failed");
        return;
    }
    static Application application({ board.display(), board.touch(), board.sd(), board.time(), board.logger() });
    if (!application.initialize())
        return;
    while (true)
        application.update();
}
