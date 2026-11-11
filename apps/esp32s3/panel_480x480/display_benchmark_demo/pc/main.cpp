// PC simulator entry point: the same application on a simulated 480x480
// screen with two scan-out buffers.
#include "../src/app/Application.h"
#include "../src/app/ApplicationConfig.h"
#include "brick/boards/pc/SimulatorBoard.h"

int main(int argc, char** argv)
{
    brick::platform::pc::SimulatorOptions options;
    if (!brick::platform::pc::SimulatorOptions::parse(argc, argv, options))
        return 2;

    brick::boards::SimulatorBoard board(brick::boards::SimulatorBoardConfig::panel480(), options);
    if (!is_ok(board.begin()))
        return 1;

    Application application(
        { board.display(), &board.display(), &board.touch(), board.memory(), board.time(), board.logger() },
        { "PC simulator 480x480", kSceneMs, kSummaryMs, kDrawMode });
    return brick::platform::pc::run(application, board.simulator());
}
