// PC simulator entry point: the same application with a speaker that counts
// the frames it plays (no sound).
#include "../src/app/Application.h"
#include "brick/boards/pc/SimulatorBoard.h"

int main(int argc, char** argv)
{
    brick::platform::pc::SimulatorOptions options;
    if (!brick::platform::pc::SimulatorOptions::parse(argc, argv, options))
        return 2;
    brick::boards::SimulatorBoard board(brick::boards::SimulatorBoardConfig::cyd(), options);
    if (!is_ok(board.begin()))
        return 1;
    Application application({ board.display(), &board.touch(), board.audio(), board.time(), board.logger() });
    return brick::platform::pc::run(application, board.simulator());
}
