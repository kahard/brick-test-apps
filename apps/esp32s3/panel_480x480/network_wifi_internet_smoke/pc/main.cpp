// PC simulator entry point: the same application with simulated Wi-Fi (it
// joins "BRICK-LAB"; --wifi changes the networks in range) and the PC's own
// network for the internet check (--offline: no internet).
#include "../src/app/Application.h"
#include "brick/boards/pc/SimulatorBoard.h"
#include "brick/platform/pc/PcPeripherals.h"
#include "brick/platform/pc/PcSockets.h"

int main(int argc, char** argv)
{
    brick::platform::pc::SimulatorOptions options;
    if (!brick::platform::pc::SimulatorOptions::parse(argc, argv, options))
        return 2;

    brick::boards::SimulatorBoard board(brick::boards::SimulatorBoardConfig::panel480(), options);
    if (!is_ok(board.begin()))
        return 1;
    brick::platform::pc::PcDnsResolver dns(options.offline);
    brick::platform::pc::PcTcpSocket   socket(options.offline);
    // The Wi-Fi setup page at http://localhost:8081/ (the device's port 80),
    // without a captive DNS; the chosen network is kept until the simulator ends.
    brick::platform::pc::PcTcpListener listener(false, 8081);
    brick::platform::pc::PcByteStorage settings(256);

    Application application({ board.display(), &board.touch(), nullptr, board.wifi().station(),
                              board.wifi().access_point(), nullptr, dns, socket, board.time(), board.logger(),
                              &listener, nullptr, &settings },
                            { "BRICK-LAB", "brick-lab-pass" });
    return brick::platform::pc::run(application, board.simulator());
}
