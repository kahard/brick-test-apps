#pragma once

namespace display_touch_smoke
{

struct DisplayTouchFeatures
{
    static constexpr bool display   = true;
    static constexpr bool touch     = true;
    static constexpr bool sd        = false;
    static constexpr bool logging   = true;
    static constexpr int  log_level = 0;
};

}  // namespace display_touch_smoke
