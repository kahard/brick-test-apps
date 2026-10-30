// CYD entry point: builds the board and the asset partition, hands them to the
// portable application and runs the loop.
#include "../app/Application.h"
#include "BoardFeatures.h"
#include "brick/boards/esp32/wroom32/CydBoard.h"
#include "brick/platform/esp32/PartitionAssetSource.h"

namespace
{
using Board = brick::boards::CydBoardTemplate<cyd_color_profile_asset_smoke::ColorProfileAssetFeatures>;
}

extern "C" void app_main()
{
    static Board                                        board;
    static brick::platform::esp32::PartitionAssetSource flash_assets{ "assets" };
    if (!board.begin() || !flash_assets.begin())
    {
        board.logger().error("brick_cyd_color_profile", "CYD board or assets partition initialization failed");
        return;
    }

    static cyd_color_profile_asset_smoke::Application application(
        { board.display(), board.touch(), flash_assets, board.memory(), board.time(), board.logger() });
    if (!application.initialize())
        return;
    while (true)
    {
        application.update();
        board.time().delay_ms(20);
    }
}
