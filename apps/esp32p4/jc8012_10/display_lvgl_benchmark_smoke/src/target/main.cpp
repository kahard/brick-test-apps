// Hardware entry point: starts the board and the asset partition, hands them
// to the portable application and runs the loop.
#include "../app/Application.h"
#include "../app/ApplicationConfig.h"
#include "brick/boards/esp32/p4/Jc8012Board.h"
#include "brick/platform/esp32/PartitionAssetSource.h"

namespace
{
using Board = brick::boards::Jc8012BoardTemplate<lvgl_benchmark_smoke::Features>;
}  // namespace

extern "C" void app_main()
{
    using namespace lvgl_benchmark_smoke;
    // Static ownership keeps board peripherals and callback targets alive even
    // if initialization fails; the ESP-IDF main task may otherwise destroy them.
    static Board                                        board;
    static brick::platform::esp32::PartitionAssetSource flash_assets{ "assets" };
    board.logger().info("application", "Starting JC8012 10-inch display_lvgl_benchmark_smoke");
    static Application application({ board.display(), flash_assets, board.memory(), board.time(), board.logger() });
    if (!board.begin() || !flash_assets.begin() || !application.initialize())
    {
        board.logger().error("application", "Initialization failed: check preceding logs, memory and assets");
        return;
    }
    board.logger().info("application", "Demo initialized");
    while (true)
        application.update();
}
