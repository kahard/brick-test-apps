#pragma once

#include <array>
#include <cstdint>

#include "ApplicationConfig.h"
#include "InternetTest.h"
#include "Services.h"

/// Portable application; the board is started by the target entry point.
class Application final
{
public:
    /// network: the Wi-Fi network to join (empty SSID: hotspot only).
    Application(const Services& services, const brick::interfaces::network::WifiCredentials& network)
        : test_(services,
                { network, kHotspotSsid, kHotspotPassword, kLineHeight, line_pixels_.data(), line_pixels_.size() })
    {
    }

    Status initialize() { return test_.initialize(); }
    void   update() { test_.update(); }

private:
    std::array<std::uint16_t, kScreenWidth* static_cast<std::size_t>(kLineHeight)> line_pixels_{};
    network_test::InternetTest                                                     test_;
};
