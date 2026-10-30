#pragma once

#include "Types.h"
#include "brick/interfaces/display/ITouchscreen.h"
#include "brick/interfaces/display/WritablePixelBuffer.h"

namespace display_touch_smoke
{
class TouchMarker final
{
public:
    void draw(brick::interfaces::display::WritablePixelBuffer& pixels, const TouchPoint& point, bool active) const;
};
}  // namespace display_touch_smoke
