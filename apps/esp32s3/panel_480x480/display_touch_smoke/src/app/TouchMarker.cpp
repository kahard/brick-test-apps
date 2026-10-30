#include "TouchMarker.h"

#include <array>
#include <cstdint>

namespace display_touch_smoke
{
namespace
{
    constexpr std::uint16_t kWidth        = 480;
    constexpr std::uint16_t kHeight       = 480;
    constexpr std::uint16_t kStripeHeight = 80;
    constexpr std::uint16_t kMarkerSize   = 100;
    constexpr std::uint16_t kMarkerArm    = 20;
    constexpr std::uint16_t kMarkerBorder = 4;
    constexpr std::uint16_t kColors[]     = { 0xF800, 0x07E0, 0x001F, 0xFFE0, 0xF81F, 0x07FF };
}  // namespace

bool TouchMarker::draw(const TouchPoint& point, bool active)
{
    static std::array<std::uint16_t, kMarkerSize * kMarkerSize> marker{};
    const auto     left = point.x < kMarkerSize / 2 ? 0 : point.x - kMarkerSize / 2;
    const auto     top  = point.y < kMarkerSize / 2 ? 0 : point.y - kMarkerSize / 2;
    const auto     x0   = left + kMarkerSize > kWidth ? kWidth - kMarkerSize : left;
    const auto     y0   = top + kMarkerSize > kHeight ? kHeight - kMarkerSize : top;
    constexpr auto half = kMarkerSize / 2;
    constexpr auto arm  = kMarkerArm / 2;
    for (std::uint16_t y = 0; y < kMarkerSize; ++y)
    {
        const auto background = static_cast<std::uint16_t>((y0 + y) / kStripeHeight);
        for (std::uint16_t x = 0; x < kMarkerSize; ++x)
        {
            const bool inner = (x >= half - arm && x < half + arm) || (y >= half - arm && y < half + arm);
            const bool outer = (x >= half - arm - kMarkerBorder && x < half + arm + kMarkerBorder)
                               || (y >= half - arm - kMarkerBorder && y < half + arm + kMarkerBorder);
            marker[static_cast<std::size_t>(y) * kMarkerSize + x] =
                active && outer ? (inner ? 0x0000 : 0xFFFF) : kColors[background < 6 ? background : 5];
        }
    }
    const brick::interfaces::display::PixelBuffer buffer{ reinterpret_cast<const std::uint8_t*>(marker.data()),
                                                          kMarkerSize,
                                                          kMarkerSize,
                                                          static_cast<std::size_t>(kMarkerSize) * sizeof(std::uint16_t),
                                                          brick::interfaces::display::PixelFormat::rgb565,
                                                          false };
    return screen_.draw({ x0, y0, kMarkerSize, kMarkerSize }, buffer) && screen_.wait_for_transfer_complete(1000);
}
}  // namespace display_touch_smoke
