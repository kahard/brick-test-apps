// Hardware entry point: starts the panel (portrait, two scan-out buffers),
// hands the board's peripherals to the portable application and runs the
// loop.
#include "../app/Application.h"
#include "../app/ApplicationConfig.h"
#include "brick/boards/esp32/p4/Jc8012Board.h"

namespace
{
using Board            = brick::boards::Jc8012BoardTemplate<BenchmarkFeatures>;
constexpr char kTag[]  = "benchmark";
constexpr char kName[] = "ESP32-P4 10\" 800x1280 MIPI DSI";
/// Internal-RAM draw buffers (DrawMode ram_and_flip and partial): 80 rows of
/// 800 pixels are 2 x 128 KB next to the 256 KB L2 cache. In ram_and_flip
/// 80 rows beat 40: half the PPA copies.
constexpr std::uint32_t kPartialRows = 80;
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
        { board.display(), &board.display(), &board.touch(), board.memory(), board.time(), board.logger() },
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
