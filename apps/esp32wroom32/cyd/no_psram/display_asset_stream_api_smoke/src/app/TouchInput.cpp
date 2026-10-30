#include "TouchInput.h"

#include "brick/interfaces/display/TouchscreenTypes.h"

namespace cyd_asset_stream_smoke
{
TouchInput::TouchInput(brick::interfaces::display::ITouchscreen& touch) : touch_(touch)
{
}

bool TouchInput::pressed()
{
    brick::interfaces::display::TouchPoint point{};
    std::size_t                            count = 0;
    if (!touch_.read(&point, 1, count) || count == 0)
        return false;
    if (point.state == brick::interfaces::display::TouchState::released)
    {
        was_down_ = false;
        return false;
    }
    const bool pressed = point.state == brick::interfaces::display::TouchState::pressed && !was_down_;
    was_down_          = true;
    return pressed;
}
}  // namespace cyd_asset_stream_smoke
