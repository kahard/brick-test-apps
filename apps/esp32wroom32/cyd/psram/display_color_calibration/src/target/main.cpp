// CYD entry point: builds the board and the console UART, hands them to the
// portable application and runs the loop.
#include "../app/Application.h"
#include "BoardFeatures.h"
#include "brick/boards/esp32/wroom32/CydBoard.h"
#include "brick/platform/esp32/EspUartPort.h"

namespace
{
using Board = brick::boards::CydBoardTemplate<cyd_color_calibration::CalibrationFeatures>;
// Large enough to hold a few 10 KB image stripes between two polls.
constexpr std::size_t kUartReceiveBytes = 20U * 1024U;
}  // namespace

extern "C" void app_main()
{
    static Board                               board;
    static brick::platform::esp32::EspUartPort uart{ UART_NUM_0, kUartReceiveBytes };
    if (!board.begin() || !uart.begin())
    {
        board.logger().error("brick_cyd_calibration", "CYD board or UART initialization failed");
        return;
    }

    static cyd_color_calibration::Application application(
        { board.display(), board.backlight(), board.touch(), uart, board.memory(), board.time(), board.logger() });
    if (!application.initialize())
        return;
    while (true)
    {
        application.update();
        board.time().delay_ms(20);
    }
}
