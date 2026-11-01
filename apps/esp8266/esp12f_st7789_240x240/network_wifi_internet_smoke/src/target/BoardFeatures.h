#pragma once

namespace network_wifi_internet_smoke
{
struct BoardFeatures
{
    static constexpr bool display   = true;
    static constexpr bool button    = true;
    static constexpr bool wifi      = true;
    static constexpr bool logging   = true;
    static constexpr int  log_level = 0;
};
}  // namespace network_wifi_internet_smoke
