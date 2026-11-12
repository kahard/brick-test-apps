#include "InternetTest.h"

#include "brick/core/network/Ipv4.h"

namespace network_test
{

using brick::interfaces::network::LinkState;

namespace
{
    constexpr char          kTag[]     = "network_test";
    constexpr std::uint32_t kScreenMs  = 250;
    constexpr std::uint16_t kWhite     = 0xFFFF;
    constexpr std::uint16_t kRed       = 0xF800;
    constexpr char          kTitle[]   = "BRICK NETWORK TEST";
    constexpr std::size_t   kTitleLine = 0;

    bool configured(const brick::interfaces::network::WifiCredentials& network)
    {
        return network.ssid != nullptr && network.ssid[0] != '\0';
    }
}  // namespace

InternetTest::InternetTest(const Services& services, const Settings& settings)
    : services_(services), settings_(settings),
      lines_(services.display, settings.line_height, settings.line_pixels, settings.line_capacity),
      wifi_(services.wifi, services.time, services.logger, &services.access_point),
      links_{ services.ethernet, &services.wifi }, network_(links_.data(), links_.size()),
      probe_(services.dns, services.socket, services.time), probe_timer_(services.time), screen_timer_(services.time)
{
    hotspot_.ssid     = settings.hotspot_ssid;
    hotspot_.password = settings.hotspot_password;
    if (services.listener != nullptr && services.settings != nullptr)
    {
        store_.emplace(*services.settings);
        portal_.emplace(wifi_, *store_, services.wifi, services.access_point, *services.listener, services.dns_socket,
                        services.time, services.logger);
    }
}

Status InternetTest::initialize()
{
    if (const Status status = lines_.clear(); !is_ok(status))
        return status;
    lines_.show(kTitleLine, kWhite, kTitle);
    if (const Status status = lines_.take_failure(); !is_ok(status))
        return status;
    if (const Status status = start_wifi_(); !is_ok(status))
        return status;
    start_portal_();
    screen_timer_.start(kScreenMs);
    show_();
    return Status::ok;
}

Status InternetTest::start_wifi_()
{
    brick::core::network::WifiManagerConfig config;
    const Status                            loaded = store_ ? store_->load() : Status::not_found;
    if (is_ok(loaded))
    {
        // Chosen on the setup page; it replaces the network from the build.
        config.networks      = store_->network();
        config.network_count = config.networks != nullptr ? 1 : 0;
        if (config.networks != nullptr)
            services_.logger.info(kTag, "Joining %s (saved on the setup page)", config.networks->ssid);
        else
            services_.logger.info(kTag, "Network forgotten on the setup page: hotspot %s only", hotspot_.ssid);
    }
    else if (configured(settings_.network))
    {
        config.networks      = &settings_.network;
        config.network_count = 1;
        services_.logger.info(kTag, "Joining %s; hotspot %s after %u failures", settings_.network.ssid, hotspot_.ssid,
                              static_cast<unsigned>(config.attempts_before_fallback));
    }
    else
    {
        services_.logger.info(kTag, "No network configured: hotspot %s only", hotspot_.ssid);
    }
    if (!is_ok(loaded) && loaded != Status::not_found)
        services_.logger.warning(kTag, "Reading the saved network failed: %s", to_string(loaded));
    config.fallback_access_point = &hotspot_;
    const Status status          = wifi_.start(config);
    if (!is_ok(status))
        services_.logger.error(kTag, "Starting Wi-Fi failed: %s", to_string(status));
    return status;
}

void InternetTest::start_portal_()
{
    if (!portal_)
        return;
    brick::core::network::WifiSetupPortalConfig config;
    config.title = hotspot_.ssid;
    if (const Status status = portal_->start(config); !is_ok(status))
        services_.logger.warning(kTag, "Setup page not started: %s", to_string(status));
    else
        services_.logger.info(kTag, "Setup page on port %u", static_cast<unsigned>(config.http_port));
}

void InternetTest::update()
{
    wifi_.update();
    if (portal_)
        portal_->update();
    if (const Status status = network_.update(); !is_ok(status))
        services_.logger.error(kTag, "Switching the default route failed: %s", to_string(status));
    log_changes_();
    const bool pressed = pressed_();
    update_hold_(pressed);
    if (pressed)
    {
        if (const Status status = wifi_.request_scan(); !is_ok(status) && status != Status::busy)
            services_.logger.warning(kTag, "Scan request failed: %s", to_string(status));
        restart_probe_();
    }
    update_probe_();
    if (screen_timer_.expired())
    {
        screen_timer_.restart();
        show_();
    }
}

void InternetTest::update_probe_()
{
    if (probe_.running())
    {
        probe_.update();
        if (!probe_.running())
            log_probe_();
        return;
    }
    if (network_.link_state() != LinkState::up || (probe_timer_.running() && !probe_timer_.expired()))
        return;
    probe_timer_.start(kProbeIntervalMs);
    if (const Status status = probe_.start(kProbeHost, kProbePath); !is_ok(status))
        services_.logger.error(kTag, "Starting the internet check failed: %s", to_string(status));
}

void InternetTest::restart_probe_()
{
    probe_.cancel();
    probe_timer_.stop();
}

void InternetTest::log_probe_()
{
    const auto& timings = probe_.timings();
    if (!is_ok(probe_.result()))
    {
        services_.logger.error(kTag, "Internet check failed after %lu ms: %s",
                               static_cast<unsigned long>(timings.total_ms), to_string(probe_.result()));
        return;
    }
    char address[brick::core::network::kIpv4TextCapacity];
    if (!is_ok(brick::core::network::format_ipv4(probe_.address(), address, sizeof(address))))
        address[0] = '\0';
    services_.logger.info(kTag, "Internet: HTTP %u from %s in %lu ms (DNS %lu ms, connect %lu ms)",
                          static_cast<unsigned>(probe_.http_status()), address,
                          static_cast<unsigned long>(timings.total_ms), static_cast<unsigned long>(timings.resolved_ms),
                          static_cast<unsigned long>(timings.connected_ms));
}

void InternetTest::log_changes_()
{
    if (wifi_.state() != logged_wifi_state_)
    {
        logged_wifi_state_ = wifi_.state();
        services_.logger.info(kTag, "Wi-Fi %s", to_string(logged_wifi_state_));
    }
    if (network_.active() == logged_link_)
        return;
    logged_link_ = network_.active();
    brick::interfaces::network::IpInfo info;
    char                               address[brick::core::network::kIpv4TextCapacity] = "-";
    if (is_ok(network_.ip_info(info))
        && !is_ok(brick::core::network::format_ipv4(info.address, address, sizeof(address))))
        address[0] = '\0';
    services_.logger.info(kTag, "Traffic goes through: %s (%s)", network_.name(), address);
    restart_probe_();
}

void InternetTest::update_hold_(bool pressed)
{
    const std::uint32_t now = services_.time.millis();
    if (pressed)
    {
        hold_started_ms_ = now;
        hold_done_       = false;
    }
    const bool          holding = portal_ && was_pressed_ && !hold_done_;
    const std::uint32_t held    = holding ? now - hold_started_ms_ : 0;
    if (holding && held >= kHoldBarDelayMs)
    {
        lines_.bar(kTitleLine, kRed, held >= kForgetHoldMs ? 1000 : held * 1000 / kForgetHoldMs);
        bar_shown_ = true;
    }
    else if (bar_shown_)
    {
        lines_.show(kTitleLine, kWhite, kTitle);
        bar_shown_ = false;
    }
    if (!holding || held < kForgetHoldMs)
        return;
    hold_done_ = true;
    services_.logger.info(kTag, "Held for %lu ms: forgetting the network", static_cast<unsigned long>(kForgetHoldMs));
    if (const Status status = portal_->forget_network(); !is_ok(status))
        services_.logger.error(kTag, "Forgetting the network failed: %s", to_string(status));
}

bool InternetTest::pressed_()
{
    if (services_.touch != nullptr)
    {
        std::size_t  count  = 0;
        const Status status = services_.touch->read(touch_points_.data(), touch_points_.size(), count);
        // no_data: no new sample since the last read (a GT911 samples about
        // every 10 ms), so the finger is where it was.
        if (status != Status::no_data)
            touched_ = is_ok(status) && count > 0
                       && touch_points_[0].state != brick::interfaces::display::TouchState::released;
    }
    const bool down    = touched_ || (services_.button != nullptr && services_.button->is_pressed());
    const bool pressed = down && !was_pressed_;
    was_pressed_       = down;
    return pressed;
}

}  // namespace network_test
