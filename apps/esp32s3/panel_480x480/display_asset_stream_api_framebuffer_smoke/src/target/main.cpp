// Hardware entry point: starts the board and the asset partition, hands them
// to the portable application and runs the loop.
#include "../app/Application.h"
#include "../app/ApplicationConfig.h"
#include "brick/boards/esp32/s3/Panel480Board.h"
#include "brick/platform/esp32/PartitionAssetSource.h"

namespace
{
using Board           = brick::boards::Panel480BoardTemplate<asset_stream_smoke::Features>;
constexpr char kTag[] = "brick_st7701s_asset_api_fb";
}  // namespace

extern "C" void app_main()
{
    using namespace asset_stream_smoke;
    static Board                                        board{ Board::double_buffered_config() };
    static brick::platform::esp32::PartitionAssetSource flash_assets{ "assets" };
    board.logger().info(kTag, "Starting partition AssetStreamer framebuffer test");
    static Application application({ board.display(), board.display(), board.touch(), flash_assets, board.sd(),
                                     board.memory(), board.time(), board.logger() });
    if (!is_ok(board.begin()) || !is_ok(flash_assets.begin()) || !is_ok(application.initialize()))
    {
        board.logger().error(kTag, "Initialization failed: check preceding logs, memory and assets");
        return;
    }
    while (true)
    {
        application.update();
        board.time().delay_ms(1);
    }
}
