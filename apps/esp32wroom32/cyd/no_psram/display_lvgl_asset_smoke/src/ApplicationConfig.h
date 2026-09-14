#pragma once

namespace cyd_lvgl_asset_smoke
{
struct LvglFeatures
{
    static constexpr bool display        = true;
    static constexpr bool touch          = true;
    static constexpr bool backlight      = true;
    static constexpr bool sd             = false;
    static constexpr bool requires_psram = false;
    static constexpr bool logging        = true;
    static constexpr int  log_level      = 0;
};
}  // namespace cyd_lvgl_asset_smoke
