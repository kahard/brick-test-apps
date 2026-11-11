// ESP-12F entry point: starts the board with its Wi-Fi radio, hands the
// peripherals and the network clients to the portable application and runs
// it from the Arduino loop.
#include <Arduino.h>

#include "../app/Application.h"
#include "BoardFeatures.h"
#include "brick/boards/esp8266/Esp12fSt7789Board.h"
#include "brick/platform/esp8266/Esp8266EepromStorage.h"
#include "brick/platform/esp8266/network/Esp8266DnsResolver.h"
#include "brick/platform/esp8266/network/Esp8266TcpListener.h"
#include "brick/platform/esp8266/network/Esp8266TcpSocket.h"
#include "brick/platform/esp8266/network/Esp8266UdpSocket.h"
#include "generated/wifi_credentials.h"

namespace
{
using Board                     = brick::boards::Esp12fSt7789BoardTemplate<network_wifi_internet_smoke::BoardFeatures>;
constexpr char          kTag[]  = "esp12f_network";
constexpr std::uint32_t kLoopMs = 10;

Board*       board       = nullptr;
Application* application = nullptr;
}  // namespace

void setup()
{
    Serial.begin(115200);
    static Board instance;
    if (const brick::Status status = instance.begin(); !is_ok(status))
    {
        instance.logger().error(kTag, "Board initialization failed: %s", to_string(status));
        return;
    }
    static brick::platform::esp8266::Esp8266DnsResolver dns;
    static brick::platform::esp8266::Esp8266TcpSocket   socket;
    // The Wi-Fi setup page, its captive DNS and the network chosen there (EEPROM sector).
    static brick::platform::esp8266::Esp8266TcpListener   listener;
    static brick::platform::esp8266::Esp8266UdpSocket     dns_socket;
    static brick::platform::esp8266::Esp8266EepromStorage settings;
    static Application test({ instance.display(), nullptr, &instance.button(), instance.wifi().station(),
                              instance.wifi().access_point(), nullptr, dns, socket, instance.time(), instance.logger(),
                              &listener, &dns_socket, &settings },
                            { kWifiSsid, kWifiPassword });
    if (const brick::Status status = test.initialize(); !is_ok(status))
    {
        instance.logger().error(kTag, "Application initialization failed: %s", to_string(status));
        return;
    }
    // Wi-Fi and lwIP take their buffers from what is left.
    instance.logger().info(kTag, "Free heap after start: %u bytes", static_cast<unsigned>(ESP.getFreeHeap()));
    board       = &instance;
    application = &test;
}

void loop()
{
    if (application == nullptr)
        return;
    application->update();
    // Yields to the Wi-Fi stack; update() itself does not block.
    board->time().delay_ms(kLoopMs);
}
