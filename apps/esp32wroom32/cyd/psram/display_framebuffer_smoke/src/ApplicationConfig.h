#pragma once

namespace cyd_display_framebuffer_smoke
{
struct FramebufferFeatures
{
    static constexpr bool display        = true;
    static constexpr bool touch          = false;
    static constexpr bool backlight      = true;
    static constexpr bool sd             = false;
    static constexpr bool requires_psram = true;
    static constexpr bool logging        = true;
    static constexpr int  log_level      = 0;
};
}  // namespace cyd_display_framebuffer_smoke
