// Hardware entry point: starts the panel with two scan-out buffers (LVGL
// draws straight into them), hands the board's peripherals to the portable
// application and runs the loop.
#include "../app/Application.h"
#include "../app/ApplicationConfig.h"
#include "brick/boards/esp32/s3/Panel480Board.h"

namespace
{
using Board            = brick::boards::Panel480BoardTemplate<BenchmarkFeatures>;
constexpr char kTag[]  = "benchmark";
constexpr char kName[] = "ESP32-S3 4\" 480x480 RGB";
}  // namespace

extern "C" void app_main()
{
    static Board board{ Board::double_buffered_config() };
    board.logger().info(kTag, "Starting the benchmark demo on %s", kName);
    if (const brick::Status status = board.begin(); !is_ok(status))
    {
        board.logger().error(kTag, "Board initialization failed: %s", to_string(status));
        return;
    }
    static Application application(
        { board.display(), &board.display(), &board.touch(), board.memory(), board.time(), board.logger() },
        { kName, kSceneMs, kSummaryMs, kDrawMode });
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
