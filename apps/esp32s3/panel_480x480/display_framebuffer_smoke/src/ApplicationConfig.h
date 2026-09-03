#pragma once

struct FramebufferFeatures
{
    static constexpr bool display   = true;
    static constexpr bool touch     = false;
    static constexpr bool sd        = false;
    static constexpr bool logging   = true;
    static constexpr int  log_level = 0;
};
