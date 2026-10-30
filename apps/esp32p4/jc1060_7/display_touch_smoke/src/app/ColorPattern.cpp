#include "ColorPattern.h"

#include <array>
#include <cstdint>

namespace display_touch_smoke
{
namespace
{
    constexpr std::uint16_t kWidth        = 1024;
    constexpr std::uint16_t kStripeHeight = 100;
    constexpr std::uint16_t kColors[]     = { 0xF800, 0x07E0, 0x001F, 0xFFE0, 0xF81F, 0x07FF };
}  // namespace

bool ColorPattern::draw()
{
    static std::array<std::uint16_t, kWidth * kStripeHeight> stripe{};
    for (std::uint16_t index = 0; index < 6; ++index)
    {
        stripe.fill(kColors[index]);
        const brick::interfaces::display::PixelBuffer buffer{ reinterpret_cast<const std::uint8_t*>(stripe.data()),
                                                              kWidth,
                                                              kStripeHeight,
                                                              static_cast<std::size_t>(kWidth) * sizeof(std::uint16_t),
                                                              brick::interfaces::display::PixelFormat::rgb565,
                                                              false };
        // Do not overwrite this stripe until the asynchronous DMA transfer ends.
        if (!screen_.draw({ 0, index * kStripeHeight, kWidth, kStripeHeight }, buffer)
            || !screen_.wait_for_transfer_complete(1000))
            return false;
    }
    return true;
}
}  // namespace display_touch_smoke
