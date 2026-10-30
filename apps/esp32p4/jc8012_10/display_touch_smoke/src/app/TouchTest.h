#pragma once

#include <array>

#include "ColorPattern.h"
#include "FrameBuffer.h"
#include "TouchMarker.h"
#include "Types.h"

namespace display_touch_smoke
{
class TouchTest final
{
public:
    explicit TouchTest(const Services& services);

    bool initialize();
    void update();

private:
    Touchscreen&              touch_;
    Logger&                   logger_;
    FrameBuffer               frame_buffer_;
    ColorPattern              color_pattern_;
    TouchMarker               touch_marker_;
    std::array<TouchPoint, 5> points_{};
    std::array<TouchPoint, 5> marker_points_{};
    std::array<bool, 5>       marker_visible_{};
};
}  // namespace display_touch_smoke
