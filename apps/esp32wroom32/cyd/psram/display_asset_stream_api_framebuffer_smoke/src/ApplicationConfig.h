#pragma once

struct AssetFeatures
{
    static constexpr bool display        = true;
    static constexpr bool touch          = true;
    static constexpr bool backlight      = true;
    static constexpr bool sd             = true;
    static constexpr bool requires_psram = true;
    static constexpr bool logging        = true;
    static constexpr int  log_level      = 0;
};
