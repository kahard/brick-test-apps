#include "TouchTest.h"

#include <cstdint>

namespace display_touch_smoke
{
namespace
{
    constexpr char TAG[] = "brick_st7701s_smoke";
}

TouchTest::TouchTest(DisplayDevice& display, Touchscreen& touch, TimeProvider& time, Logger& logger)
    : touch_(touch), logger_(logger), screen_(display), color_pattern_(screen_), touch_marker_(screen_),
      frame_timer_(time)
{
}

bool TouchTest::initialize()
{
    color_pattern_.draw();
    frame_timer_.start(16);
    return true;
}

void TouchTest::update()
{
    std::size_t count = 0;
    if (touch_.read(points_.data(), points_.size(), count))
    {
        for (std::size_t index = 0; index < count; ++index)
        {
            const auto& point  = points_[index];
            const bool  active = point.state != brick::interfaces::display::TouchState::released;
            const bool moved = active && (!marker_visible_ || marker_point_.x != point.x || marker_point_.y != point.y);
            if (!active)
            {
                if (marker_visible_)
                    touch_marker_.draw(marker_point_, false);
                marker_visible_ = false;
            }
            else if (moved)
            {
                if (marker_visible_)
                    touch_marker_.draw(marker_point_, false);
                marker_point_   = point;
                marker_visible_ = true;
                touch_marker_.draw(marker_point_, true);
            }
            logger_.info(TAG, "touch id=%u x=%d y=%d raw=(%d,%d) pressure=%d state=%u", point.id, point.x, point.y,
                         point.raw_x, point.raw_y, point.pressure, static_cast<unsigned>(point.state));
        }
    }
    if (frame_timer_.expired())
        frame_timer_.restart();
}
}  // namespace display_touch_smoke
