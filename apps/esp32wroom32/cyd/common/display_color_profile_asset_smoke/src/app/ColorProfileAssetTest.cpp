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
    // A second tap within this time centres the divider.
    constexpr std::uint32_t kDoubleTapMs = 400U;

    const brick::interfaces::display::AssetDescriptor& asset()
    {
        return generated_assets::entries[0];
    }
}  // namespace

ColorProfileAssetTest::ColorProfileAssetTest(const Services& services)
    : display_(services.display), time_(services.time), logger_(services.logger), touch_(services.touch),
      assets_(services.assets), stripe_(services.memory), divider_(kWidth - 1U, kDoubleTapMs)
{
}

Status ColorProfileAssetTest::initialize()
{
    logger_.info(kTag, "Color profile asset: left=source, right=corrected; drag divider, double-tap=center");
    if (const Status status = stripe_.allocate(kStripeBytes, brick::interfaces::memory::MemoryKind::dma);
        !is_ok(status))
    {
        logger_.error(kTag, "DMA stripe allocation failed: %s", to_string(status));
        return status;
    }
    return render();
}

void ColorProfileAssetTest::update()
{
    update_divider();
}

Status ColorProfileAssetTest::render()
{
    std::uint8_t* const                            stripe  = stripe_.data();
    const brick::core::display::Rgb565ColorProfile profile = cyd_color_profile::profile();
    const brick::interfaces::display::PixelBuffer  buffer{
        stripe, kWidth, kStripeHeight, kWidth * sizeof(std::uint16_t), brick::interfaces::display::PixelFormat::rgb565,
        true
    };
    const std::uint16_t divider_x = divider_.position();
    Status              status    = Status::ok;
    for (std::uint16_t y = 0U; y < kHeight && is_ok(status); y += kStripeHeight)
    {
        status =
            assets_.read(asset(), static_cast<std::size_t>(y) * kWidth * sizeof(std::uint16_t), stripe, kStripeBytes);
        if (!is_ok(status))
            break;
        auto* pixels = reinterpret_cast<std::uint16_t*>(stripe);
        for (std::uint16_t row = 0U; row < kStripeHeight; ++row)
            for (std::uint16_t x = divider_x; x < kWidth; ++x)
            {
                const std::size_t index = static_cast<std::size_t>(row) * kWidth + x;
                pixels[index]           = profile.apply(pixels[index]);
            }
        for (std::uint16_t row = 0U; row < kStripeHeight; ++row)
        {
            const std::size_t row_offset = static_cast<std::size_t>(row) * kWidth;
            if (divider_x < kWidth)
                pixels[row_offset + divider_x] = 0x0000U;
            if (divider_x > 0U)
                pixels[row_offset + divider_x - 1U] = 0x0000U;
        }
        status = display_.draw_buffer({ 0, y, kWidth, kStripeHeight }, buffer);
        if (is_ok(status))
            status = display_.wait_for_transfer_complete(1000U);
    }
    return status;
}

void ColorProfileAssetTest::redraw()
{
    if (const Status status = render(); !is_ok(status))
        logger_.error(kTag, "Redraw failed: %s", to_string(status));
}

void ColorProfileAssetTest::update_divider()
{
    std::array<brick::interfaces::display::TouchPoint, 1U> points{};
    std::size_t                                            count = 0U;
    if (!is_ok(touch_.read(points.data(), points.size(), count)) || count == 0U)
        return;
    if (divider_.update(points[0], time_.millis()))
        redraw();
}

}  // namespace cyd_color_profile_asset_smoke
