// PC simulator entry point: the same application on a simulated CYD screen;
// --serial-in replays what the calibration GUI would send.
#include "../src/app/Application.h"
#include "brick/boards/pc/SimulatorBoard.h"

int main(int argc, char** argv)
{
    brick::platform::pc::SimulatorOptions options;
    if (!brick::platform::pc::SimulatorOptions::parse(argc, argv, options))
        return 2;

    brick::boards::SimulatorBoard board(brick::boards::SimulatorBoardConfig::cyd(), options);
    if (!board.begin())
        return 1;

    cyd_color_calibration::Application application({ board.display(), board.backlight(), board.touch(), board.serial(),
                                                     board.memory(), board.time(), board.logger() });
    return brick::platform::pc::run(application, board.simulator());
}
