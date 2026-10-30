#include "ColorProfileAssetTest.h"

#include <array>
#include <cstddef>

#include "CydColorCalibration.h"
#include "brick/core/display/Rgb565ColorProfile.h"
#include "brick/interfaces/display/PixelBuffer.h"
#include "generated_assets.h"

namespace cyd_color_profile_asset_smoke
{
namespace
{
    constexpr char          kTag[]        = "brick_cyd_color_profile";
    constexpr std::uint16_t kWidth        = 320U;
    constexpr std::uint16_t kHeight       = 240U;
    constexpr std::uint16_t kStripeHeight = 16U;
    constexpr std::size_t   kStripeBytes  = static_cast<std::size_t>(kWidth) * kStripeHeight * sizeof(std::uint16_t);

    const brick::interfaces::display::AssetDescriptor& asset()
    {
        return generated_assets::entries[0];
    }
}  // namespace

ColorProfileAssetTest::ColorProfileAssetTest(const Services& services)
    : display_(services.display), time_(services.time), logger_(services.logger), touch_(services.touch),
      assets_(services.assets), stripe_(services.memory)
{
}

bool ColorProfileAssetTest::initialize()
{
    logger_.info(kTag, "Color profile asset: left=source, right=corrected; drag divider, double-tap=center");
    if (!stripe_.allocate(kStripeBytes, brick::interfaces::memory::MemoryKind::dma))
    {
        logger_.error(kTag, "DMA stripe allocation failed");
        return false;
    }
    return render();
}

void ColorProfileAssetTest::update()
{
    update_divider();
}

bool ColorProfileAssetTest::render()
{
    std::uint8_t* const                            stripe  = stripe_.data();
    const brick::core::display::Rgb565ColorProfile profile = cyd_color_profile::profile();
    const brick::interfaces::display::PixelBuffer  buffer{
        stripe, kWidth, kStripeHeight, kWidth * sizeof(std::uint16_t), brick::interfaces::display::PixelFormat::rgb565,
        true
    };
    bool success = true;
    for (std::uint16_t y = 0U; y < kHeight && success; y += kStripeHeight)
    {
        if (!assets_.read(asset(), static_cast<std::size_t>(y) * kWidth * sizeof(std::uint16_t), stripe, kStripeBytes))
        {
            success = false;
            break;
        }
        auto* pixels = reinterpret_cast<std::uint16_t*>(stripe);
        for (std::uint16_t row = 0U; row < kStripeHeight; ++row)
            for (std::uint16_t x = divider_x_; x < kWidth; ++x)
            {
                const std::size_t index = static_cast<std::size_t>(row) * kWidth + x;
                pixels[index]           = profile.apply(pixels[index]);
            }
        for (std::uint16_t row = 0U; row < kStripeHeight; ++row)
        {
            const std::size_t row_offset = static_cast<std::size_t>(row) * kWidth;
            if (divider_x_ < kWidth)
                pixels[row_offset + divider_x_] = 0x0000U;
            if (divider_x_ > 0U)
                pixels[row_offset + divider_x_ - 1U] = 0x0000U;
        }
        success =
            display_.draw_buffer({ 0, y, kWidth, kStripeHeight }, buffer) && display_.wait_for_transfer_complete(1000U);
    }
    if (!success)
        logger_.error(kTag, "Asset read or display transfer failed");
    return success;
}

void ColorProfileAssetTest::update_divider()
{
    std::array<brick::interfaces::display::TouchPoint, 1U> points{};
    std::size_t                                            count = 0U;
    if (!touch_.read(points.data(), points.size(), count) || count == 0U)
        return;

    constexpr std::int16_t maximum_x = kWidth - 1U;
    const std::int16_t     x         = points[0].x < 0 ? 0 : (points[0].x > maximum_x ? maximum_x : points[0].x);
    if (points[0].state == brick::interfaces::display::TouchState::released)
    {
        dragging_ = false;
        return;
    }
    if (points[0].state == brick::interfaces::display::TouchState::pressed)
    {
        const std::uint32_t now = time_.millis();
        if (last_tap_ms_ != 0U && now - last_tap_ms_ <= 400U)
        {
            divider_x_   = kWidth / 2U;
            last_tap_ms_ = 0U;
            render();
        }
        else
            last_tap_ms_ = now;
        last_touch_x_ = x;
        dragging_     = true;
        return;
    }
    if (!dragging_)
    {
        last_touch_x_ = x;
        dragging_     = true;
        return;
    }
    const std::int16_t delta      = static_cast<std::int16_t>(x - last_touch_x_);
    last_touch_x_                 = x;
    const std::int16_t  requested = static_cast<std::int16_t>(divider_x_) + delta;
    const std::uint16_t next =
        static_cast<std::uint16_t>(requested < 0 ? 0 : (requested > maximum_x ? maximum_x : requested));
    if (next != divider_x_)
    {
        divider_x_ = next;
        render();
    }
}

}  // namespace cyd_color_profile_asset_smoke
