// PC simulator entry point: the same application on a simulated screen.
#include "../src/app/Application.h"
#include "brick/boards/pc/SimulatorBoard.h"

int main(int argc, char** argv)
{
    using namespace display_touch_smoke;
    brick::platform::pc::SimulatorOptions options;
    if (!brick::platform::pc::SimulatorOptions::parse(argc, argv, options))
        return 2;

    brick::boards::SimulatorBoard board(brick::boards::SimulatorBoardConfig::jc1060(), options);
    if (!board.begin())
        return 1;

    Application application({ board.display(), board.touch(), board.time(), board.logger() });
    return brick::platform::pc::run(application, board.simulator());
}
