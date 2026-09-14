#pragma once

struct LvglFeatures
{
    static constexpr bool display        = true;
    static constexpr bool touch          = true;
    static constexpr bool backlight      = true;
    static constexpr bool sd             = false;
    static constexpr bool requires_psram = true;
    static constexpr bool logging        = true;
    static constexpr int  log_level      = 0;
};
