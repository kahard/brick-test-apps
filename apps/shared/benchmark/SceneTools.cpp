#include "SceneTools.h"

namespace benchmark_demo
{

namespace
{
    constexpr std::int32_t kLargeFontMinWidth = 400;
    /// A longer pause (a slow frame, a breakpoint) does not throw objects across the screen.
    constexpr std::uint32_t kMaxStepMs             = 100;
    constexpr std::int32_t  kMillisecondsPerSecond = 1000;
    constexpr std::uint32_t kColorWheelSegment     = 256;
    constexpr std::uint32_t kChannelMax            = 255;

    lv_color_t rgb(std::uint32_t red, std::uint32_t green, std::uint32_t blue)
    {
        return lv_color_make(static_cast<std::uint8_t>(red), static_cast<std::uint8_t>(green),
                             static_cast<std::uint8_t>(blue));
    }
}  // namespace

Size content_size(lv_obj_t* parent)
{
    lv_obj_update_layout(parent);
    return { lv_obj_get_content_width(parent), lv_obj_get_content_height(parent) };
}

lv_obj_t* create_box(lv_obj_t* parent, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height)
{
    lv_obj_t* box = lv_obj_create(parent);
    lv_obj_remove_style_all(box);
    lv_obj_remove_flag(box, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_flag(box, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_pos(box, x, y);
    lv_obj_set_size(box, width, height);
    return box;
}

lv_obj_t* create_filled_box(lv_obj_t* parent, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height,
                            lv_color_t color)
{
    lv_obj_t* box = create_box(parent, x, y, width, height);
    lv_obj_set_style_bg_color(box, color, 0);
    lv_obj_set_style_bg_opa(box, LV_OPA_COVER, 0);
    return box;
}

lv_obj_t* create_label(lv_obj_t* parent, const lv_font_t* font, lv_color_t color)
{
    lv_obj_t* label = lv_label_create(parent);
    lv_obj_remove_flag(label, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_text_font(label, font, 0);
    lv_obj_set_style_text_color(label, color, 0);
    return label;
}

const lv_font_t* small_font()
{
    return &lv_font_montserrat_14;
}

const lv_font_t* large_font(std::int32_t screen_width)
{
    return screen_width >= kLargeFontMinWidth ? &lv_font_montserrat_20 : &lv_font_montserrat_14;
}

std::uint32_t Random::next()
{
    state_ ^= state_ << 13U;
    state_ ^= state_ >> 17U;
    state_ ^= state_ << 5U;
    return state_;
}

std::int32_t Random::range(std::int32_t low, std::int32_t high)
{
    if (high <= low)
        return low;
    const auto span = static_cast<std::uint32_t>(high - low) + 1U;
    return low + static_cast<std::int32_t>(next() % span);
}

void Bouncer::reset(std::int32_t x, std::int32_t y, std::int32_t speed_x, std::int32_t speed_y, std::int32_t max_x,
                    std::int32_t max_y)
{
    constexpr std::int32_t kOne = 1 << kFractionBits;
    max_x_                      = (max_x > 0 ? max_x : 0) * kOne;
    max_y_                      = (max_y > 0 ? max_y : 0) * kOne;
    x_                          = x * kOne;
    y_                          = y * kOne;
    speed_x_                    = speed_x;
    speed_y_                    = speed_y;
    remainder_x_                = 0;
    remainder_y_                = 0;
}

void Bouncer::step(std::uint32_t elapsed_ms)
{
    const std::uint32_t elapsed = elapsed_ms < kMaxStepMs ? elapsed_ms : kMaxStepMs;
    move_(x_, speed_x_, remainder_x_, max_x_, elapsed);
    move_(y_, speed_y_, remainder_y_, max_y_, elapsed);
}

void Bouncer::move_(std::int32_t& position, std::int32_t& speed, std::int32_t& remainder, std::int32_t limit,
                    std::uint32_t elapsed_ms)
{
    if (limit <= 0)
    {
        position = 0;
        return;
    }
    // Sub-pixel movement adds up over steps of a millisecond or two.
    remainder += speed * static_cast<std::int32_t>(elapsed_ms) * (1 << kFractionBits);
    position += remainder / kMillisecondsPerSecond;
    remainder %= kMillisecondsPerSecond;
    if (position < 0)
    {
        position = -position;
        speed    = -speed;
    }
    else if (position > limit)
    {
        position = 2 * limit - position;
        speed    = -speed;
    }
    position = position < 0 ? 0 : (position > limit ? limit : position);
}

std::int32_t triangle_wave(std::uint32_t time_ms, std::uint32_t period_ms, std::int32_t peak)
{
    if (period_ms < 2U)
        return 0;
    const std::uint32_t phase = time_ms % period_ms;
    const std::uint32_t half  = period_ms / 2U;
    if (phase < half)
        return static_cast<std::int32_t>(static_cast<std::uint32_t>(peak) * phase / half);
    return static_cast<std::int32_t>(static_cast<std::uint32_t>(peak) * (period_ms - phase) / (period_ms - half));
}

lv_color_t color_wheel(std::uint32_t phase)
{
    const std::uint32_t step    = phase % kColorWheelSteps;
    const std::uint32_t segment = step / kColorWheelSegment;
    const std::uint32_t rise    = step % kColorWheelSegment;
    const std::uint32_t fall    = kChannelMax - rise;
    switch (segment)
    {
        case 0:
            return rgb(kChannelMax, rise, 0);
        case 1:
            return rgb(fall, kChannelMax, 0);
        case 2:
            return rgb(0, kChannelMax, rise);
        case 3:
            return rgb(0, fall, kChannelMax);
        case 4:
            return rgb(rise, 0, kChannelMax);
        default:
            return rgb(kChannelMax, 0, fall);
    }
}

}  // namespace benchmark_demo
