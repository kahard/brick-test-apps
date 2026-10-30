#pragma once

#include "brick/core/display/Screen.h"

namespace cyd_display_touch_smoke
{
class ColorPattern final
{
public:
    explicit ColorPattern(brick::core::display::Screen& screen) : screen_(screen) {}

    void draw();

private:
    brick::core::display::Screen& screen_;
};
}  // namespace cyd_display_touch_smoke
