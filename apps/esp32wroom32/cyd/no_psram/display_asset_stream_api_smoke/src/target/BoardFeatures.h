#pragma once

namespace cyd_asset_stream_smoke
{
struct AssetFeatures
{
    static constexpr bool display        = true;
    static constexpr bool touch          = true;
    static constexpr bool backlight      = true;
    static constexpr bool sd             = false;
    static constexpr bool requires_psram = false;
    static constexpr bool logging        = true;
    static constexpr int  log_level      = 0;
};
}  // namespace cyd_asset_stream_smoke
