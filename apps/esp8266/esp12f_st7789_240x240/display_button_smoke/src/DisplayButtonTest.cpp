#include "DisplayButtonTest.h"

#include "brick/interfaces/display/PixelBuffer.h"

namespace esp12f_st7789_button_smoke
{
namespace
{
    constexpr std::uint16_t kWidth          = 240;
    constexpr std::uint16_t kStripeHeight   = 40;
    constexpr std::uint16_t kStatusHeight   = 16;
    constexpr std::uint32_t kPollIntervalMs = 20;
}  // namespace

DisplayButtonTest::DisplayButtonTest(DisplayDevice& display, Button& button, TimeProvider& time, Logger& logger)
    : display_(display), button_(button), time_(time), logger_(logger)
{
}

bool DisplayButtonTest::initialize()
{
    if (!draw_pattern())
        return false;
    last_pressed_ = button_.is_pressed();
    if (!draw_button_state(last_pressed_))
        return false;
    logger_.info("button_smoke", "ready; GPIO4 pressed=%u", last_pressed_ ? 1U : 0U);
    return true;
}

void DisplayButtonTest::update()
{
    if (time_.millis() < next_poll_ms_)
        return;
    next_poll_ms_      = time_.millis() + kPollIntervalMs;
    const bool pressed = button_.is_pressed();
    if (pressed == last_pressed_)
        return;
    last_pressed_ = pressed;
    if (draw_button_state(pressed))
        logger_.info("button_smoke", "GPIO4 %s", pressed ? "pressed" : "released");
}

bool DisplayButtonTest::draw_rows(std::uint16_t y, std::uint16_t height, std::uint16_t color)
{
    for (std::size_t index = 0; index < static_cast<std::size_t>(kWidth) * height; ++index)
    {
        pixels_[index * 2U]      = static_cast<std::uint8_t>(color >> 8U);
        pixels_[index * 2U + 1U] = static_cast<std::uint8_t>(color);
    }
    const brick::interfaces::display::PixelBuffer buffer{ pixels_.data(),
                                                          kWidth,
                                                          height,
                                                          static_cast<std::size_t>(kWidth) * 2U,
                                                          brick::interfaces::display::PixelFormat::rgb565,
                                                          false };
    return display_.draw_buffer({ 0, y, kWidth, height }, buffer);
}

bool DisplayButtonTest::draw_pattern()
{
    constexpr std::array<std::uint16_t, 6> colors{ 0xF800, 0x07E0, 0x001F, 0xFFE0, 0xF81F, 0x07FF };
    for (std::uint16_t index = 0; index < colors.size(); ++index)
        if (!draw_rows(index * kStripeHeight, kStripeHeight, colors[index]))
            return false;
    return true;
}

bool DisplayButtonTest::draw_button_state(bool pressed)
{
    return draw_rows(kWidth - kStatusHeight, kStatusHeight, pressed ? 0x07E0 : 0x4208);
}
}  // namespace esp12f_st7789_button_smoke
