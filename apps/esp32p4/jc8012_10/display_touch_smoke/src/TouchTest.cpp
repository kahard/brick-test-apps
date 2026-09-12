#include "TouchTest.h"

#include <cstddef>

namespace display_touch_smoke
{
namespace
{
    constexpr char TAG[] = "brick_jc8012_touch";
}

TouchTest::TouchTest(DisplayDevice& display, Touchscreen& touch, Logger& logger)
    : touch_(touch), logger_(logger), frame_buffer_(display)
{
}

bool TouchTest::initialize()
{
    if (!frame_buffer_.initialize())
    {
        logger_.error(TAG, "Unable to allocate a PSRAM framebuffer");
        return false;
    }

    color_pattern_.fill(frame_buffer_.pixels());
    if (!frame_buffer_.present())
    {
        logger_.error(TAG, "Initial color pattern presentation failed");
        return false;
    }
    return true;
}

void TouchTest::update()
{
    std::size_t count = 0;
    if (!touch_.read(points_.data(), points_.size(), count))
        return;

    bool frame_dirty = false;
    for (std::size_t index = 0; index < count; ++index)
    {
        const TouchPoint& point = points_[index];
        if (point.id >= marker_visible_.size())
            continue;

        const bool active = point.state != brick::interfaces::display::TouchState::released;
        const bool moved  = active
                           && (!marker_visible_[point.id] || marker_points_[point.id].x != point.x
                               || marker_points_[point.id].y != point.y);
        if (!active)
        {
            if (marker_visible_[point.id])
            {
                touch_marker_.draw(frame_buffer_.pixels(), marker_points_[point.id], false);
                frame_dirty = true;
            }
            marker_visible_[point.id] = false;
        }
        else if (moved)
        {
            if (marker_visible_[point.id])
                touch_marker_.draw(frame_buffer_.pixels(), marker_points_[point.id], false);
            marker_points_[point.id]  = point;
            marker_visible_[point.id] = true;
            frame_dirty               = true;
        }

        logger_.info(TAG, "touch id=%u x=%d y=%d raw=(%d,%d) pressure=%d state=%u", point.id, point.x, point.y,
                     point.raw_x, point.raw_y, point.pressure, static_cast<unsigned>(point.state));
    }

    if (!frame_dirty)
        return;

    for (std::size_t id = 0; id < marker_visible_.size(); ++id)
        if (marker_visible_[id])
            touch_marker_.draw(frame_buffer_.pixels(), marker_points_[id], true);
    if (!frame_buffer_.present())
        logger_.warning(TAG, "Touch framebuffer presentation failed");
}
}  // namespace display_touch_smoke
