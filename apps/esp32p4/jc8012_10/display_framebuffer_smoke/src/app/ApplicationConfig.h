#pragma once

namespace display_framebuffer_smoke
{
struct Features
{
    static constexpr bool display   = true;
    static constexpr bool touch     = false;
    static constexpr bool backlight = true;
    static constexpr bool sd        = false;
    static constexpr bool logging   = true;
    static constexpr int  log_level = 0;
};
static_assert(Features::display, "This demo requires the display.");
}  // namespace display_framebuffer_smoke
