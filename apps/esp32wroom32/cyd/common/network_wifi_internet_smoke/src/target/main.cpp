// Hardware entry point: starts the board with its network interfaces, hands
// the peripherals and the network clients to the portable application and
// runs the loop.
#include "../app/Application.h"
#include "BoardFeatures.h"
#include "brick/boards/esp32/wroom32/CydBoard.h"
#include "brick/platform/esp32/network/EspDnsResolver.h"
#include "brick/platform/esp32/network/EspTcpSocket.h"
#include "generated/wifi_credentials.h"

namespace
{
using Board                     = brick::boards::CydBoardTemplate<network_wifi_internet_smoke::BoardFeatures>;
constexpr char          kTag[]  = "cyd_network";
constexpr std::uint32_t kLoopMs = 10;
}  // namespace

extern "C" void app_main()
{
    static Board board;
    if (const brick::Status status = board.begin(); !is_ok(status))
    {
        board.logger().error(kTag, "Board initialization failed: %s", to_string(status));
        return;
    }
    static brick::platform::esp32::EspDnsResolver dns;
    static brick::platform::esp32::EspTcpSocket   socket;
    static Application application({ board.display(), &board.touch(), nullptr, board.wifi().station(),
                                     board.wifi().access_point(), nullptr, dns, socket, board.time(), board.logger() },
                                   { kWifiSsid, kWifiPassword });
    if (const brick::Status status = application.initialize(); !is_ok(status))
    {
        board.logger().error(kTag, "Application initialization failed: %s", to_string(status));
        return;
    }
    // The application's update() does not block; the loop paces it.
    while (true)
    {
        application.update();
        board.time().delay_ms(kLoopMs);
    }
}
