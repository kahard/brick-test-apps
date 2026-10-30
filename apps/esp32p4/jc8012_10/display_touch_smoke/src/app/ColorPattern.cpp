#include "ColorPattern.h"

#include <cstdint>

namespace display_touch_smoke
{
namespace
{
    constexpr std::uint16_t kStripeCount           = 8;
    constexpr std::uint16_t kOrientationMarkerSize = 120;
    constexpr std::uint16_t kColors[]              = { 0xF800, 0x07E0, 0x001F, 0xFFE0, 0xF81F, 0x07FF, 0x0000, 0xFFFF };
}  // namespace

void ColorPattern::fill(brick::interfaces::display::WritablePixelBuffer& pixels) const
{
    std::uint16_t* const data = reinterpret_cast<std::uint16_t*>(pixels.data);
    for (std::uint16_t y = 0; y < pixels.height; ++y)
    {
        for (std::uint16_t x = 0; x < pixels.width; ++x)
            data[static_cast<std::size_t>(y) * pixels.width + x] = background_color(x, y, pixels.height);
    }
}

std::uint16_t ColorPattern::background_color(std::uint32_t x, std::uint32_t y, std::uint32_t height)
{
    if (x < kOrientationMarkerSize && y >= height - kOrientationMarkerSize)
        return 0x0000;

    const std::uint32_t stripe_height = height / kStripeCount;
    const std::uint32_t stripe        = y / stripe_height;
    return kColors[stripe < kStripeCount ? stripe : kStripeCount - 1U];
}
}  // namespace display_touch_smoke
