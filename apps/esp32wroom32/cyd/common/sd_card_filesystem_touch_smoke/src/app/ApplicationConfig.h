#pragma once

struct SdFeatures
{
    static constexpr bool display        = true;
    static constexpr bool touch          = true;
    static constexpr bool backlight      = true;
    static constexpr bool sd             = true;
    static constexpr bool requires_psram = false;
    static constexpr bool logging        = true;
    static constexpr int  log_level      = 0;
};
