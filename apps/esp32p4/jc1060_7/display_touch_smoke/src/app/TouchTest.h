#pragma once

#include <array>

#include "ColorPattern.h"
#include "TouchMarker.h"
#include "Types.h"
#include "brick/core/display/Screen.h"
#include "brick/core/time/Timer.h"
#include "brick/interfaces/display/ITouchscreen.h"
#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/time/ITimeProvider.h"

namespace display_touch_smoke
{
class TouchTest final
{
public:
    TouchTest(DisplayDevice& display, Touchscreen& touch, TimeProvider& time, Logger& logger);

    bool initialize();
    void update();

private:
    Touchscreen&                 touch_;
    Logger&                      logger_;
    brick::core::display::Screen screen_;
    ColorPattern                 color_pattern_;
    TouchMarker                  touch_marker_;
    brick::core::time::Timer     frame_timer_;
    std::array<TouchPoint, 5>    points_{};
    TouchPoint                   marker_point_{};
    bool                         marker_visible_ = false;
};
}  // namespace display_touch_smoke
