#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

#include "NetworkServices.h"
#include "TextLines.h"
#include "brick/core/network/HttpProbe.h"
#include "brick/core/network/NetworkSelector.h"
#include "brick/core/network/WifiCredentialStore.h"
#include "brick/core/network/WifiManager.h"
#include "brick/core/network/WifiSetupPortal.h"
#include "brick/core/time/Timer.h"

namespace network_test
{

/// Joins the configured Wi-Fi network (opening a hotspot when that fails or
/// none is configured), prefers Ethernet when the board has it, and checks
/// the internet with an HTTP probe every kProbeIntervalMs. Everything shows
/// on the screen and in the log; a touch or a button press scans again and
/// repeats the probe. With a listener and settings storage it also serves
/// the Wi-Fi setup page (on the hotspot and at the device's address): a
/// network chosen there is saved and replaces the one from the build; a
/// touch or button held kForgetHoldMs forgets it again.
class InternetTest final
{
public:
    static constexpr char          kProbeHost[]     = "connectivitycheck.gstatic.com";
    static constexpr char          kProbePath[]     = "/generate_204";
    static constexpr std::uint16_t kExpectedStatus  = 204;
    static constexpr std::uint32_t kProbeIntervalMs = 30000;
    /// A touch or button held this long forgets the network chosen on the
    /// setup page (as "Forget" there) and opens the hotspot; a red bar in
    /// place of the title fills up meanwhile (after kHoldBarDelayMs).
    static constexpr std::uint32_t kForgetHoldMs   = 5000;
    static constexpr std::uint32_t kHoldBarDelayMs = 300;

    InternetTest(const Services& services, const Settings& settings);

    Status initialize();
    void   update();

private:
    Status      start_wifi_();
    void        start_portal_();
    void        update_probe_();
    void        restart_probe_();
    void        log_probe_();
    void        log_changes_();
    bool        pressed_();
    void        update_hold_(bool pressed);
    void        show_();
    std::size_t show_links_(std::size_t line);
    std::size_t show_internet_(std::size_t line);
    std::size_t show_hotspot_(std::size_t line);
    void        show_networks_(std::size_t line);

    Services                                                      services_;
    Settings                                                      settings_;
    TextLines                                                     lines_;
    brick::core::network::WifiManager                             wifi_;
    std::optional<brick::core::network::WifiCredentialStore>      store_;
    std::optional<brick::core::network::WifiSetupPortal>          portal_;
    std::array<brick::interfaces::network::INetworkInterface*, 2> links_;
    brick::core::network::NetworkSelector                         network_;
    brick::core::network::HttpProbe                               probe_;
    brick::core::time::Timer                                      probe_timer_;
    brick::core::time::Timer                                      screen_timer_;
    brick::interfaces::network::WifiAccessPointConfig             hotspot_{};
    brick::core::network::WifiManagerState         logged_wifi_state_ = brick::core::network::WifiManagerState::stopped;
    brick::interfaces::network::INetworkInterface* logged_link_       = nullptr;
    std::array<brick::interfaces::display::TouchPoint, 1> touch_points_{};
    bool                                                  was_pressed_     = false;
    bool                                                  touched_         = false;  ///< the last touch sample
    bool                                                  hold_done_       = false;
    bool                                                  bar_shown_       = false;
    std::uint32_t                                         hold_started_ms_ = 0;
};

}  // namespace network_test
