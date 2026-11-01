// PC simulator entry point: the same application on a simulated CYD screen; the
// generated bundle file stands in for the flash partition.
#include "../src/app/Application.h"
#include "brick/boards/pc/SimulatorBoard.h"
#include "brick/platform/pc/PcFileAssetSource.h"

int main(int argc, char** argv)
{
    brick::platform::pc::SimulatorOptions options;
    if (!brick::platform::pc::SimulatorOptions::parse(argc, argv, options))
        return 2;

    brick::boards::SimulatorBoard          board(brick::boards::SimulatorBoardConfig::cyd(), options);
    brick::platform::pc::PcFileAssetSource flash_assets(BRICK_PC_ASSET_BUNDLE);
    if (!is_ok(board.begin()) || !is_ok(flash_assets.begin()))
        return 1;

    cyd_asset_stream_smoke::Application application(
        { board.display(), board.touch(), flash_assets, board.sd(), board.memory(), board.time(), board.logger() });
    return brick::platform::pc::run(application, board.simulator());
}
