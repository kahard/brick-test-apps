#pragma once

#include "Types.h"
#include "brick/core/display/Screen.h"
#include "brick/interfaces/display/ITouchscreen.h"

namespace cyd_display_touch_smoke
{
class TouchMarker final
{
public:
    explicit TouchMarker(brick::core::display::Screen& screen) : screen_(screen) {}

    bool draw(const TouchPoint& point, bool active);

private:
    brick::core::display::Screen& screen_;
};
}  // namespace cyd_display_touch_smoke
