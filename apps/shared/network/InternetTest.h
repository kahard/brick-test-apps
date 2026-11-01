#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "NetworkServices.h"
#include "TextLines.h"
#include "brick/core/network/HttpProbe.h"
#include "brick/core/network/NetworkSelector.h"
#include "brick/core/network/WifiManager.h"
#include "brick/core/time/Timer.h"

namespace network_test
{

/// Joins the configured Wi-Fi network (opening a hotspot when that fails or
/// none is configured), prefers Ethernet when the board has it, and checks
/// the internet with an HTTP probe every kProbeIntervalMs. Everything shows
/// on the screen and in the log; a touch or a button press scans again and
/// repeats the probe.
class InternetTest final
{
public:
    static constexpr char          kProbeHost[]     = "connectivitycheck.gstatic.com";
    static constexpr char          kProbePath[]     = "/generate_204";
    static constexpr std::uint16_t kExpectedStatus  = 204;
    static constexpr std::uint32_t kProbeIntervalMs = 30000;

    InternetTest(const Services& services, const Settings& settings);

    Status initialize();
    void   update();

private:
    Status      start_wifi_();
    void        update_probe_();
    void        restart_probe_();
    void        log_probe_();
    void        log_changes_();
    bool        pressed_();
    void        show_();
    std::size_t show_links_(std::size_t line);
    std::size_t show_internet_(std::size_t line);
    std::size_t show_hotspot_(std::size_t line);
    void        show_networks_(std::size_t line);

    Services                                                      services_;
    Settings                                                      settings_;
    TextLines                                                     lines_;
    brick::core::network::WifiManager                             wifi_;
    std::array<brick::interfaces::network::INetworkInterface*, 2> links_;
    brick::core::network::NetworkSelector                         network_;
    brick::core::network::HttpProbe                               probe_;
    brick::core::time::Timer                                      probe_timer_;
    brick::core::time::Timer                                      screen_timer_;
    brick::interfaces::network::WifiAccessPointConfig             hotspot_{};
    brick::core::network::WifiManagerState         logged_wifi_state_ = brick::core::network::WifiManagerState::stopped;
    brick::interfaces::network::INetworkInterface* logged_link_       = nullptr;
    std::array<brick::interfaces::display::TouchPoint, 1> touch_points_{};
    bool                                                  was_pressed_ = false;
};

}  // namespace network_test
