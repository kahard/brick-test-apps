#include "SpriteImage.h"

namespace benchmark_demo
{

namespace
{
    constexpr std::int32_t kMinDiameter = 8;
    constexpr std::int32_t kChannelMax  = 255;
    /// Base colour of the ball and how far the light reaches.
    constexpr std::int32_t kRed          = 255;
    constexpr std::int32_t kGreen        = 140;
    constexpr std::int32_t kBlue         = 40;
    constexpr std::int32_t kShadeRange   = 3;   ///< darkest at three radii squared from the light
    constexpr std::int32_t kSpotFraction = 16;  ///< the white spot covers 1/16 of the radius squared

    std::int32_t clamp_channel(std::int32_t value)
    {
        return value < 0 ? 0 : (value > kChannelMax ? kChannelMax : value);
    }
}  // namespace

Status SpriteImage::initialize(std::int32_t diameter)
{
    diameter_         = diameter < kMinDiameter ? kMinDiameter : diameter;
    const auto side   = static_cast<std::size_t>(diameter_);
    const auto stride = side * kBytesPerPixel;
    if (const Status status = pixels_.allocate(stride * side, brick::interfaces::memory::MemoryKind::internal);
        !is_ok(status))
        return status;
    draw_();
    descriptor_.header.magic  = LV_IMAGE_HEADER_MAGIC;
    descriptor_.header.cf     = LV_COLOR_FORMAT_ARGB8888;
    descriptor_.header.flags  = 0;
    descriptor_.header.w      = static_cast<std::uint32_t>(side);
    descriptor_.header.h      = static_cast<std::uint32_t>(side);
    descriptor_.header.stride = static_cast<std::uint32_t>(stride);
    descriptor_.data_size     = static_cast<std::uint32_t>(stride * side);
    descriptor_.data          = pixels_.data();
    return Status::ok;
}

void SpriteImage::draw_()
{
    std::uint8_t* pixel = pixels_.data();
    for (std::int32_t y = 0; y < diameter_; ++y)
        for (std::int32_t x = 0; x < diameter_; ++x, pixel += kBytesPerPixel)
            draw_pixel_(x, y, pixel);
}

void SpriteImage::draw_pixel_(std::int32_t x, std::int32_t y, std::uint8_t* pixel) const
{
    // Coordinates in half pixels from the centre: the edge is at distance diameter.
    const std::int32_t dx       = 2 * x + 1 - diameter_;
    const std::int32_t dy       = 2 * y + 1 - diameter_;
    const std::int32_t distance = dx * dx + dy * dy;
    const std::int32_t outer    = diameter_ * diameter_;
    const std::int32_t inner    = (diameter_ - 2) * (diameter_ - 2);
    std::int32_t       alpha    = kChannelMax;
    if (distance >= outer)
        alpha = 0;
    else if (distance > inner)
        alpha = kChannelMax * (outer - distance) / (outer - inner);
    // The light comes from the top left.
    const std::int32_t lx    = dx + diameter_ / 3;
    const std::int32_t ly    = dy + diameter_ / 3;
    const std::int32_t light = lx * lx + ly * ly;
    const std::int32_t shade = kChannelMax - clamp_channel(light * kChannelMax / (kShadeRange * outer));
    const std::int32_t spot  = clamp_channel(kChannelMax - light * kChannelMax * kSpotFraction / outer);
    const std::int32_t red   = kRed * shade / kChannelMax;
    const std::int32_t green = kGreen * shade / kChannelMax;
    const std::int32_t blue  = kBlue * shade / kChannelMax;
    // LVGL keeps ARGB8888 as blue, green, red, alpha bytes.
    pixel[0] = static_cast<std::uint8_t>(clamp_channel(blue + (kChannelMax - blue) * spot / kChannelMax));
    pixel[1] = static_cast<std::uint8_t>(clamp_channel(green + (kChannelMax - green) * spot / kChannelMax));
    pixel[2] = static_cast<std::uint8_t>(clamp_channel(red + (kChannelMax - red) * spot / kChannelMax));
    pixel[3] = static_cast<std::uint8_t>(alpha);
}

}  // namespace benchmark_demo
