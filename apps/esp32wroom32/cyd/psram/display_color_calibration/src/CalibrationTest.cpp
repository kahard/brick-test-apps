#include "CalibrationTest.h"

#include <array>

#include "brick/interfaces/display/PixelBuffer.h"

namespace cyd_color_calibration
{
namespace
{

    constexpr char          kTag[]         = "brick_cyd_calibration";
    constexpr std::uint16_t kDisplayHeight = 240U;

}  // namespace

CalibrationTest::CalibrationTest(brick::interfaces::display::IDisplayDevice& display,
                                 brick::interfaces::display::IBacklight&     backlight,
                                 brick::interfaces::time::ITimeProvider&     time,
                                 brick::interfaces::logging::ILogger&        logger,
                                 brick::interfaces::display::ITouchscreen&   touch)
    : display_(display), backlight_(backlight), time_(time), logger_(logger), touch_(touch)
{
}

bool CalibrationTest::initialize()
{
    if (!buffer_.initialize() || !image_.initialize() || !protocol_.initialize())
    {
        logger_.error(kTag, "DMA buffer, PSRAM image cache, or UART receiver unavailable");
        return false;
    }
    logger_.info(kTag, "Calibration ready: upload an image, then drag the black divider horizontally");
    return true;
}

void CalibrationTest::update()
{
    protocol_.poll();
    apply_pending_commands();
    update_image_divider();
    time_.delay_ms(20U);
}

bool CalibrationTest::render()
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

        if (!display_.draw_buffer({ 0, y, CalibrationBuffer::width(), CalibrationBuffer::height() }, buffer)
            || !display_.wait_for_transfer_complete(1000U))
        {
            logger_.error(kTag, "Display transfer failed");
            return false;
        }
        time_.delay_ms(10U);
    }
    return true;
}

bool CalibrationTest::render_image_stripe(std::uint8_t stripe_index, const std::uint8_t* pixels)
{
    if (pixels == nullptr)
        return false;
    for (std::size_t index = 0U; index < CalibrationBuffer::width() * CalibrationBuffer::height(); ++index)
    {
        const std::size_t   byte_index = index * 2U;
        const std::uint16_t source     = static_cast<std::uint16_t>(pixels[byte_index])
                                     | (static_cast<std::uint16_t>(pixels[byte_index + 1U]) << 8U);
        const std::uint16_t x       = static_cast<std::uint16_t>(index % CalibrationBuffer::width());
        const bool          divider = x == divider_x_ || (divider_x_ > 0U && x == divider_x_ - 1U);
        buffer_.pixels()[index]     = divider ? 0x0000U : (x < divider_x_ ? source : profile_.apply(source));
    }
    const brick::interfaces::display::PixelBuffer buffer{ reinterpret_cast<const std::uint8_t*>(buffer_.pixels()),
                                                          CalibrationBuffer::width(),
                                                          CalibrationBuffer::height(),
                                                          CalibrationBuffer::width() * sizeof(std::uint16_t),
                                                          brick::interfaces::display::PixelFormat::rgb565,
                                                          true };
    const std::uint16_t                           y = stripe_index * CalibrationBuffer::height();
    return display_.draw_buffer({ 0, y, CalibrationBuffer::width(), CalibrationBuffer::height() }, buffer)
           && display_.wait_for_transfer_complete(1000U);
}

bool CalibrationTest::render_cached_image()
{
    if (!image_.complete())
        return false;

    for (std::uint8_t stripe_index = 0U; stripe_index < CalibrationImage::stripe_count(); ++stripe_index)
    {
        if (!render_image_stripe(stripe_index, image_.stripe(stripe_index)))
            return false;
        time_.delay_ms(10U);
    }
    return true;
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
    std::uint8_t brightness = 0U;
    if (protocol_.consume_brightness_percent(brightness))
    {
        backlight_.set_brightness_percent(brightness);
        logger_.info(kTag, "UART brightness=%u", brightness);
    }

    bool redraw_pattern = false;
    if (protocol_.consume_profile(profile_))
    {
        redraw_pattern = !showing_custom_image_;
        logger_.info(kTag, "UART RGB565 profile received");
        if (showing_custom_image_ && !render_cached_image())
            logger_.error(kTag, "Cached image redraw failed or image is incomplete");
    }

    std::uint8_t pattern = 0U;
    if (protocol_.consume_pattern(pattern))
    {
        pattern_              = static_cast<std::uint8_t>(pattern % 3U);
        showing_custom_image_ = false;
        redraw_pattern        = true;
        logger_.info(kTag, "UART pattern=%u", pattern_);
    }
    std::uint8_t        stripe_index = 0U;
    const std::uint8_t* image_pixels = nullptr;
    if (protocol_.consume_image_stripe(stripe_index, image_pixels))
    {
        showing_custom_image_ = true;
        logger_.info(kTag, "UART image stripe=%u", stripe_index);
        if (!image_.store_stripe(stripe_index, image_pixels))
            logger_.error(kTag, "Image stripe cache failed");
        else if (image_.complete())
        {
            logger_.info(kTag, "Reference image complete; drag the black divider on CYD");
            if (!render_cached_image())
                logger_.error(kTag, "Reference image render failed");
        }
    }
    if (redraw_pattern)
        render();
}

void CalibrationTest::update_image_divider()
{
    if (!showing_custom_image_ || !image_.complete())
        return;

    std::array<brick::interfaces::display::TouchPoint, 1U> points{};
    std::size_t                                            count = 0U;
    if (!touch_.read(points.data(), points.size(), count) || count == 0U)
        return;

    const std::int16_t maximum_x = static_cast<std::int16_t>(CalibrationImage::width() - 1U);
    const std::int16_t clamped_x = points[0].x < 0 ? 0 : (points[0].x > maximum_x ? maximum_x : points[0].x);
    if (points[0].state == brick::interfaces::display::TouchState::released)
    {
        touch_dragging_ = false;
        return;
    }

    if (points[0].state == brick::interfaces::display::TouchState::pressed)
    {
        const std::uint32_t now = time_.millis();
        if (last_tap_ms_ != 0U && now - last_tap_ms_ <= 400U)
        {
            divider_x_   = CalibrationImage::width() / 2U;
            last_tap_ms_ = 0U;
            logger_.info(kTag, "Divider centered");
            if (!render_cached_image())
                logger_.error(kTag, "Image divider redraw failed");
        }
        else
            last_tap_ms_ = now;

        last_touch_x_   = clamped_x;
        touch_dragging_ = true;
        return;
    }

    if (!touch_dragging_)
    {
        last_touch_x_   = clamped_x;
        touch_dragging_ = true;
        return;
    }

    const std::int16_t delta = static_cast<std::int16_t>(clamped_x - last_touch_x_);
    last_touch_x_            = clamped_x;
    if (delta == 0)
        return;

    const std::int16_t  requested = static_cast<std::int16_t>(divider_x_) + delta;
    const std::uint16_t requested_divider =
        static_cast<std::uint16_t>(requested < 0 ? 0 : (requested > maximum_x ? maximum_x : requested));
    if (requested_divider == divider_x_)
        return;

    divider_x_ = requested_divider;
    logger_.info(kTag, "Divider x=%u", divider_x_);
    if (!render_cached_image())
        logger_.error(kTag, "Image divider redraw failed");
}

}  // namespace cyd_color_calibration
