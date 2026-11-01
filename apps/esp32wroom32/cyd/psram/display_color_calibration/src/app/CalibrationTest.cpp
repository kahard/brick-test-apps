#include "CalibrationTest.h"

#include <array>

#include "brick/interfaces/display/PixelBuffer.h"

namespace cyd_color_calibration
{
namespace
{

    constexpr char          kTag[]         = "brick_cyd_calibration";
    constexpr std::uint16_t kDisplayHeight = 240U;
    constexpr std::uint8_t  kPatternCount  = 3U;
    // A second tap within this time centres the divider.
    constexpr std::uint32_t kDoubleTapMs = 400U;

}  // namespace

CalibrationTest::CalibrationTest(const Services& services)
    : display_(services.display), backlight_(services.backlight), time_(services.time), logger_(services.logger),
      touch_(services.touch), buffer_(services.memory), image_(services.memory), protocol_(services.serial),
      divider_(CalibrationImage::width() - 1U, kDoubleTapMs)
{
}

Status CalibrationTest::initialize()
{
    Status status = buffer_.initialize();
    if (is_ok(status))
        status = image_.initialize();
    if (!is_ok(status))
    {
        logger_.error(kTag, "DMA buffer or PSRAM image cache unavailable: %s", to_string(status));
        return status;
    }
    logger_.info(kTag, "Calibration ready: upload an image, then drag the black divider horizontally");
    return Status::ok;
}

void CalibrationTest::update()
{
    protocol_.poll();
    apply_pending_commands();
    update_image_divider();
}

Status CalibrationTest::render()
{
    brick::interfaces::display::PixelBuffer buffer{ reinterpret_cast<const std::uint8_t*>(buffer_.pixels()),
                                                    CalibrationBuffer::width(),
                                                    CalibrationBuffer::height(),
                                                    CalibrationBuffer::width() * sizeof(std::uint16_t),
                                                    brick::interfaces::display::PixelFormat::rgb565,
                                                    true };

    for (std::uint16_t y = 0U; y < kDisplayHeight; y += CalibrationBuffer::height())
    {
        for (std::uint16_t row = 0U; row < CalibrationBuffer::height(); ++row)
            for (std::uint16_t x = 0U; x < CalibrationBuffer::width(); ++x)
                buffer_.pixels()[static_cast<std::size_t>(row) * CalibrationBuffer::width() + x] =
                    profile_.apply(source_pixel(x, static_cast<std::uint16_t>(y + row)));

        if (!is_ok(display_.draw_buffer({ 0, y, CalibrationBuffer::width(), CalibrationBuffer::height() }, buffer))
            || !is_ok(display_.wait_for_transfer_complete(1000U)))
        {
            logger_.error(kTag, "Display transfer failed");
            return Status::error;
        }
    }
    return Status::ok;
}

Status CalibrationTest::render_image_stripe(std::uint8_t stripe_index, const std::uint8_t* pixels)
{
    if (pixels == nullptr)
        return Status::error;
    for (std::size_t index = 0U; index < CalibrationBuffer::width() * CalibrationBuffer::height(); ++index)
    {
        const std::size_t   byte_index = index * 2U;
        const std::uint16_t source     = static_cast<std::uint16_t>(pixels[byte_index])
                                     | (static_cast<std::uint16_t>(pixels[byte_index + 1U]) << 8U);
        const std::uint16_t x         = static_cast<std::uint16_t>(index % CalibrationBuffer::width());
        const std::uint16_t divider_x = divider_.position();
        const bool          divider   = x == divider_x || (divider_x > 0U && x == divider_x - 1U);
        buffer_.pixels()[index]       = divider ? 0x0000U : (x < divider_x ? source : profile_.apply(source));
    }
    const brick::interfaces::display::PixelBuffer buffer{ reinterpret_cast<const std::uint8_t*>(buffer_.pixels()),
                                                          CalibrationBuffer::width(),
                                                          CalibrationBuffer::height(),
                                                          CalibrationBuffer::width() * sizeof(std::uint16_t),
                                                          brick::interfaces::display::PixelFormat::rgb565,
                                                          true };
    const std::uint16_t                           y = stripe_index * CalibrationBuffer::height();
    if (const Status status =
            display_.draw_buffer({ 0, y, CalibrationBuffer::width(), CalibrationBuffer::height() }, buffer);
        !is_ok(status))
        return status;
    return display_.wait_for_transfer_complete(1000U);
}

Status CalibrationTest::render_cached_image()
{
    if (!image_.complete())
        return Status::error;

    for (std::uint8_t stripe_index = 0U; stripe_index < CalibrationImage::stripe_count(); ++stripe_index)
    {
        if (!is_ok(render_image_stripe(stripe_index, image_.stripe(stripe_index))))
            return Status::error;
    }
    return Status::ok;
}

std::uint16_t CalibrationTest::source_pixel(std::uint16_t x, std::uint16_t y) const
{
    if (pattern_ == 1U)
    {
        constexpr std::array<std::uint16_t, 8U> colors{ 0xFFFFU, 0xFFE0U, 0x07FFU, 0x07E0U,
                                                        0xF81FU, 0xF800U, 0x001FU, 0x0000U };
        return colors[(static_cast<std::size_t>(x) * colors.size()) / CalibrationBuffer::width()];
    }
    if (pattern_ == 2U)
    {
        const std::uint16_t level =
            static_cast<std::uint16_t>((static_cast<std::uint32_t>(x) * 31U) / (CalibrationBuffer::width() - 1U));
        if (y < kDisplayHeight / 3U)
            return static_cast<std::uint16_t>(level << 11U);
        if (y < (2U * kDisplayHeight) / 3U)
            return static_cast<std::uint16_t>((level << 1U) << 5U);
        return level;
    }

    const std::uint16_t red =
        static_cast<std::uint16_t>((static_cast<std::uint32_t>(x) * 31U) / (CalibrationBuffer::width() - 1U));
    const std::uint16_t green =
        static_cast<std::uint16_t>((static_cast<std::uint32_t>(x) * 63U) / (CalibrationBuffer::width() - 1U));
    const std::uint16_t blue = red;
    return static_cast<std::uint16_t>((red << 11U) | (green << 5U) | blue);
}

void CalibrationTest::apply_pending_commands()
{
    apply_brightness();
    const bool profile_changed = apply_profile();
    const bool pattern_changed = apply_pattern();
    apply_image_stripe();
    if (profile_changed || pattern_changed)
        if (const Status status = render(); !is_ok(status))
            logger_.error(kTag, "Pattern render failed: %s", to_string(status));
}

void CalibrationTest::apply_brightness()
{
    std::uint8_t brightness = 0U;
    if (!protocol_.consume_brightness_percent(brightness))
        return;
    if (const Status status = backlight_.set_brightness_percent(brightness); is_ok(status))
        logger_.info(kTag, "UART brightness=%u", brightness);
    else
        logger_.error(kTag, "UART brightness=%u rejected: %s", brightness, to_string(status));
}

// True when the pattern on screen has to be redrawn with the new profile.
bool CalibrationTest::apply_profile()
{
    if (!protocol_.consume_profile(profile_))
        return false;
    logger_.info(kTag, "UART RGB565 profile received");
    if (!showing_custom_image_)
        return true;
    if (const Status status = render_cached_image(); !is_ok(status))
        logger_.error(kTag, "Cached image redraw failed: %s", to_string(status));
    return false;
}

bool CalibrationTest::apply_pattern()
{
    std::uint8_t pattern = 0U;
    if (!protocol_.consume_pattern(pattern))
        return false;
    pattern_              = static_cast<std::uint8_t>(pattern % kPatternCount);
    showing_custom_image_ = false;
    logger_.info(kTag, "UART pattern=%u", pattern_);
    return true;
}

void CalibrationTest::apply_image_stripe()
{
    std::uint8_t        stripe_index = 0U;
    const std::uint8_t* image_pixels = nullptr;
    if (!protocol_.consume_image_stripe(stripe_index, image_pixels))
        return;
    showing_custom_image_ = true;
    logger_.info(kTag, "UART image stripe=%u", stripe_index);
    if (const Status status = image_.store_stripe(stripe_index, image_pixels); !is_ok(status))
    {
        logger_.error(kTag, "Image stripe cache failed: %s", to_string(status));
        return;
    }
    if (!image_.complete())
        return;
    logger_.info(kTag, "Reference image complete; drag the black divider on CYD");
    if (const Status status = render_cached_image(); !is_ok(status))
        logger_.error(kTag, "Reference image render failed: %s", to_string(status));
}

void CalibrationTest::update_image_divider()
{
    if (!showing_custom_image_ || !image_.complete())
        return;
    std::array<brick::interfaces::display::TouchPoint, 1U> points{};
    std::size_t                                            count = 0U;
    if (!is_ok(touch_.read(points.data(), points.size(), count)) || count == 0U)
        return;
    if (!divider_.update(points[0], time_.millis()))
        return;
    logger_.info(kTag, "Divider x=%u", divider_.position());
    if (const Status status = render_cached_image(); !is_ok(status))
        logger_.error(kTag, "Image divider redraw failed: %s", to_string(status));
}

}  // namespace cyd_color_calibration
