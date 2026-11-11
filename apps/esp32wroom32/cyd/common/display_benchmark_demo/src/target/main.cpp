// Hardware entry point: starts the CYD's ILI9341 (320x240 over SPI) and its
// touch screen, hands them to the portable application and runs the loop.
#include "../app/Application.h"
#include "../app/ApplicationConfig.h"
#include "brick/boards/esp32/wroom32/CydBoard.h"

namespace
{
using Board            = brick::boards::CydBoardTemplate<BenchmarkFeatures>;
constexpr char kTag[]  = "benchmark";
constexpr char kName[] = "ESP32 CYD 2.8\" 320x240 SPI";
/// Two LVGL draw buffers of 40 rows of 320 pixels (2 x 25 KB) in DMA-capable
/// internal RAM: the SPI DMA cannot read PSRAM on the ESP32.
constexpr std::uint32_t kPartialRows = 40;
}  // namespace

extern "C" void app_main()
{
    static Board board;
    board.logger().info(kTag, "Starting the benchmark demo on %s", kName);
    if (const brick::Status status = board.begin(); !is_ok(status))
    {
        board.logger().error(kTag, "Board initialization failed: %s", to_string(status));
        return;
    }
    static Application application(
        { board.display(), nullptr, &board.touch(), board.memory(), board.time(), board.logger() },
        { kName, kSceneMs, kSummaryMs, kDrawMode, kPartialRows });
    if (const brick::Status status = application.initialize(); !is_ok(status))
    {
        board.logger().error(kTag, "Application initialization failed: %s", to_string(status));
        return;
    }
    while (true)
    {
        application.update();
        board.time().delay_ms(1);
    }
}
