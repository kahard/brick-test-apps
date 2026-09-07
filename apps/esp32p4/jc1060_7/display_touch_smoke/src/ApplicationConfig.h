#pragma once

namespace display_touch_smoke
{
struct Features
{
    static constexpr bool display   = true;
    static constexpr bool touch     = true;
    static constexpr bool backlight = true;
    static constexpr bool sdmmc     = false;
    static constexpr bool logging   = true;
    static constexpr int  log_level = 0;
};

static_assert(Features::display && Features::touch && Features::backlight,
              "The display/touch demo requires display, touch and backlight.");
}  // namespace display_touch_smoke
