// CYD entry point: builds the board and the asset partition, hands them to the
// portable application and runs the loop.
#include "../app/Application.h"
#include "BoardFeatures.h"
#include "brick/boards/esp32/wroom32/CydBoard.h"
#include "brick/platform/esp32/PartitionAssetSource.h"

namespace
{
using Board = brick::boards::CydBoardTemplate<cyd_asset_stream_smoke::AssetFeatures>;
}

extern "C" void app_main()
{
    static Board                                        board;
    static brick::platform::esp32::PartitionAssetSource flash_assets{ "assets" };
    if (!is_ok(board.begin()) || !is_ok(flash_assets.begin()))
    {
        board.logger().error("cyd_asset_stream", "CYD board or assets partition initialization failed");
        return;
    }

    static cyd_asset_stream_smoke::Application application(
        { board.display(), board.touch(), flash_assets, board.memory(), board.time(), board.logger() });
    if (const brick::Status status = application.initialize(); !is_ok(status))
    {
        board.logger().error("cyd_asset_stream", "Application initialization failed: %s", to_string(status));
        return;
    }
    while (true)
    {
        application.update();
        board.time().delay_ms(10);
    }
}
