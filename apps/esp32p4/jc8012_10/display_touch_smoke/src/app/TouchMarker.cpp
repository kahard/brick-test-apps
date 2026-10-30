#include "TouchMarker.h"

#include <cstddef>
#include <cstdint>

#include "ColorPattern.h"

namespace display_touch_smoke
{
namespace
{
    constexpr std::uint16_t kMarkerSize   = 100;
    constexpr std::uint16_t kMarkerArm    = 20;
    constexpr std::uint16_t kMarkerBorder = 4;
}  // namespace

void TouchMarker::draw(brick::interfaces::display::WritablePixelBuffer& pixels, const TouchPoint& point,
                       bool active) const
{
    const std::int32_t      left = point.x < kMarkerSize / 2 ? 0 : point.x - kMarkerSize / 2;
    const std::int32_t      top  = point.y < kMarkerSize / 2 ? 0 : point.y - kMarkerSize / 2;
    const std::int32_t      x0   = left + kMarkerSize > pixels.width ? pixels.width - kMarkerSize : left;
    const std::int32_t      y0   = top + kMarkerSize > pixels.height ? pixels.height - kMarkerSize : top;
    constexpr std::uint16_t half = kMarkerSize / 2;
    constexpr std::uint16_t arm  = kMarkerArm / 2;
    std::uint16_t* const    data = reinterpret_cast<std::uint16_t*>(pixels.data);
    for (std::uint16_t y = 0; y < kMarkerSize; ++y)
    {
        for (std::uint16_t x = 0; x < kMarkerSize; ++x)
        {
            const bool inner = (x >= half - arm && x < half + arm) || (y >= half - arm && y < half + arm);
            const bool outer = (x >= half - arm - kMarkerBorder && x < half + arm + kMarkerBorder)
                               || (y >= half - arm - kMarkerBorder && y < half + arm + kMarkerBorder);
            data[static_cast<std::size_t>(y0 + y) * pixels.width + x0 + x] =
                active && outer ? (inner ? 0x0000 : 0xFFFF) :
                                  ColorPattern::background_color(x0 + x, y0 + y, pixels.height);
        }
    }
}
}  // namespace display_touch_smoke
