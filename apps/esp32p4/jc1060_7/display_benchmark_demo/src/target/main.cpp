// Hardware entry point: starts the panel (1024x600, two scan-out buffers),
// hands the board's peripherals to the portable application and runs the
// loop.
#include "../app/Application.h"
#include "../app/ApplicationConfig.h"
#include "brick/boards/esp32/p4/Jc1060Board.h"

namespace
{
using Board            = brick::boards::Jc1060BoardTemplate<BenchmarkFeatures>;
constexpr char kTag[]  = "benchmark";
constexpr char kName[] = "ESP32-P4 7\" 1024x600 MIPI DSI";
/// Internal-RAM draw buffers (DrawMode ram_and_flip and partial): 48 rows of
/// 1024 pixels are 2 x 96 KB. Two 128 KB buffers do not fit: the largest free
/// DMA-capable block is about 250 KB next to the 256 KB L2 cache.
constexpr std::uint32_t kPartialRows = 48;
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
