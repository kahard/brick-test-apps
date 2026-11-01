// Hardware entry point: starts the board, hands its peripherals to the
// portable application and runs the loop.
#include "../app/Application.h"
#include "../app/ApplicationConfig.h"
#include "brick/boards/esp32/wroom32/CydBoard.h"

namespace
{
using Board = brick::boards::CydBoardTemplate<SdFeatures>;
}  // namespace

extern "C" void app_main()
{
    using namespace cyd_sd_card_filesystem_touch_smoke;
    static Board board;
    if (!board.begin())
        return;

    static Application application({ board.display(), board.sd(), board.touch(), board.time(), board.logger() });
    if (!application.initialize())
        return;
    // The application's update() does not block; the loop paces it.
    while (true)
    {
        application.update();
        board.time().delay_ms(30);
    }
}
