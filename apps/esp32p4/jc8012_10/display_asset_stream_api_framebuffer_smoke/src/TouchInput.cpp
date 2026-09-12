#include "TouchInput.h"

TouchInput::TouchInput(brick::interfaces::display::ITouchscreen& touch) : touch_(touch)
{
}

bool TouchInput::pressed()
{
    brick::interfaces::display::TouchPoint point{};
    std::size_t                            count = 0;
    // No new sample is not a release. Preserve the edge detector state.
    if (!touch_.read(&point, 1, count))
        return false;
    const bool is_down = count > 0 && point.state != brick::interfaces::display::TouchState::released;
    const bool pressed = is_down && !was_down_;
    was_down_          = is_down;
    return pressed;
}
