// The screen of InternetTest: one line per link, the internet check, the
// hotspot and the networks of the last scan.
#include "InternetTest.h"
#include "brick/core/network/Ipv4.h"

namespace network_test
{

using brick::core::network::WifiManagerState;
using brick::interfaces::network::LinkState;

namespace
{
    constexpr char          kTag[]         = "network_test";
    constexpr std::size_t   kFirstInfoLine = 1;
    constexpr std::uint16_t kWhite         = 0xFFFF;
    constexpr std::uint16_t kGray          = 0x8410;
    constexpr std::uint16_t kGreen         = 0x07E0;
    constexpr std::uint16_t kYellow        = 0xFFE0;
    constexpr std::uint16_t kRed           = 0xF800;
    constexpr std::uint16_t kCyan          = 0x07FF;

    std::uint16_t color_of(LinkState state)
    {
        return state == LinkState::up ? kGreen : state == LinkState::connecting ? kYellow : kGray;
    }

    std::uint16_t color_of(WifiManagerState state)
    {
        switch (state)
        {
            case WifiManagerState::connected:
                return kGreen;
            case WifiManagerState::scanning:
            case WifiManagerState::connecting:
                return kYellow;
            case WifiManagerState::waiting:
                return kRed;
            case WifiManagerState::stopped:
                break;
        }
        return kGray;
    }

    const char* address_text(const brick::interfaces::network::INetworkInterface& link, char* text, std::size_t size)
    {
        brick::interfaces::network::IpInfo info;
        if (!is_ok(link.ip_info(info)) || !is_ok(brick::core::network::format_ipv4(info.address, text, size)))
            return "-";
        return text;
    }
}  // namespace

void InternetTest::show_()
{
    std::size_t line = show_links_(kFirstInfoLine);
    line             = show_internet_(line);
    line             = show_hotspot_(line);
    show_networks_(line);
    // Logged once per failure; the next refresh tries again.
    if (const Status status = lines_.take_failure(); !is_ok(status))
        services_.logger.error(kTag, "Drawing the screen failed: %s", to_string(status));
}

std::size_t InternetTest::show_links_(std::size_t line)
{
    const WifiManagerState                  state = wifi_.state();
    brick::interfaces::network::WifiNetwork joined;
    if (state == WifiManagerState::connected && is_ok(services_.wifi.connected_network(joined)))
        lines_.show(line++, kGreen, "WIFI  %s  %d dBm  ch %u", joined.ssid, joined.rssi_dbm,
                    static_cast<unsigned>(joined.channel));
    else if (state == WifiManagerState::waiting)
        lines_.show(line++, kRed, "WIFI  %s (%u)", to_string(wifi_.last_failure()),
                    static_cast<unsigned>(wifi_.failed_attempts()));
    else
        lines_.show(line++, color_of(state), "WIFI  %s %s", to_string(state),
                    wifi_.current_network() != nullptr ? wifi_.current_network()->ssid : "");
    if (services_.ethernet != nullptr)
    {
        const LinkState ethernet = services_.ethernet->link_state();
        lines_.show(line++, color_of(ethernet), "ETHERNET  %s",
                    ethernet == LinkState::down ? "down (no cable?)" : to_string(ethernet));
    }
    char address[brick::core::network::kIpv4TextCapacity];
    lines_.show(line++, network_.active() != nullptr ? kWhite : kGray, "IP  %s  via %s",
                address_text(network_, address, sizeof(address)), network_.name());
    return line;
}

std::size_t InternetTest::show_internet_(std::size_t line)
{
    const auto& timings = probe_.timings();
    if (probe_.running())
        lines_.show(line, kYellow, "INTERNET  checking...");
    else if (probe_.result() == Status::not_ready)
        lines_.show(line, kGray, "INTERNET  waiting for a link");
    else if (!is_ok(probe_.result()))
        lines_.show(line, kRed, "INTERNET  FAIL: %s", to_string(probe_.result()));
    else if (probe_.http_status() == kExpectedStatus)
        lines_.show(line, kGreen, "INTERNET  OK  HTTP %u  %lu ms", static_cast<unsigned>(probe_.http_status()),
                    static_cast<unsigned long>(timings.total_ms));
    else
        lines_.show(line, kYellow, "INTERNET  HTTP %u (captive portal?)", static_cast<unsigned>(probe_.http_status()));
    return line + 1;
}

std::size_t InternetTest::show_hotspot_(std::size_t line)
{
    if (!wifi_.access_point_active())
    {
        lines_.show(line, kGray, "HOTSPOT  off");
        return line + 1;
    }
    // The SSID, the password, then where to reach the device: short lines for small screens.
    char address[brick::core::network::kIpv4TextCapacity];
    lines_.show(line++, kCyan, "HOTSPOT  %s", hotspot_.ssid);
    lines_.show(line++, kCyan, "  password %s", hotspot_.password != nullptr ? hotspot_.password : "(none)");
    lines_.show(line++, kCyan, "  %s  %u clients", address_text(services_.access_point, address, sizeof(address)),
                static_cast<unsigned>(services_.access_point.client_count()));
    return line;
}

void InternetTest::show_networks_(std::size_t line)
{
    lines_.show(line++, kWhite, "NETWORKS  %u", static_cast<unsigned>(wifi_.network_count()));
    for (std::size_t index = 0; line < lines_.count(); ++index, ++line)
    {
        if (index >= wifi_.network_count())
        {
            lines_.show(line, kGray, " ");
            continue;
        }
        const auto& network = wifi_.networks()[index];
        lines_.show(line, kWhite, "%s  %d dBm  ch %u  %s", network.ssid[0] != '\0' ? network.ssid : "(hidden)",
                    network.rssi_dbm, static_cast<unsigned>(network.channel), to_string(network.security));
    }
}

}  // namespace network_test
