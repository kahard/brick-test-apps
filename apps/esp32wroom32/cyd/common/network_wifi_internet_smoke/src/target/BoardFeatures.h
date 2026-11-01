#pragma once

namespace network_wifi_internet_smoke
{
struct BoardFeatures
{
    static constexpr bool display        = true;
    static constexpr bool touch          = true;
    static constexpr bool backlight      = true;
    static constexpr bool sd             = false;
    static constexpr bool requires_psram = false;
    static constexpr bool wifi           = true;
    static constexpr bool logging        = true;
    static constexpr int  log_level      = 0;
};
}  // namespace network_wifi_internet_smoke
