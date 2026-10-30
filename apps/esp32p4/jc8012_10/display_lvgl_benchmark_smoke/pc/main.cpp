// PC simulator entry point: the same application on a simulated screen; the
// generated bundle file stands in for the flash partition.
#include "../src/app/Application.h"
#include "brick/boards/pc/SimulatorBoard.h"
#include "brick/platform/pc/PcFileAssetSource.h"

int main(int argc, char** argv)
{
    using namespace lvgl_benchmark_smoke;
    brick::platform::pc::SimulatorOptions options;
    if (!brick::platform::pc::SimulatorOptions::parse(argc, argv, options))
        return 2;

    brick::boards::SimulatorBoard          board(brick::boards::SimulatorBoardConfig::jc8012(), options);
    brick::platform::pc::PcFileAssetSource flash_assets(BRICK_PC_ASSET_BUNDLE);
    if (!board.begin() || !flash_assets.begin())
        return 1;

    Application application({ board.display(), flash_assets, board.memory(), board.time(), board.logger() });
    return brick::platform::pc::run(application, board.simulator());
}
