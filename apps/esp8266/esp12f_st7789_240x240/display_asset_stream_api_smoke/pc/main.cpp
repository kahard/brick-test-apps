// PC simulator entry point: the same application on a simulated ESP-12F
// ST7789 screen (space = the touch button).
#include "../src/app/Application.h"
#include "brick/boards/pc/SimulatorBoard.h"

int main(int argc, char** argv)
{
    brick::platform::pc::SimulatorOptions options;
    if (!brick::platform::pc::SimulatorOptions::parse(argc, argv, options))
        return 2;

    brick::boards::SimulatorBoard board(brick::boards::SimulatorBoardConfig::esp12f_st7789(), options);
    if (!is_ok(board.begin()))
        return 1;

    esp12f_st7789_asset_stream_api_smoke::Application application({ board.display(), board.time(), board.logger() });
    return brick::platform::pc::run(application, board.simulator());
}
