#pragma once

#include <cstdint>

#include "lvgl.h"

namespace benchmark_demo
{

struct Size
{
    std::int32_t width  = 0;
    std::int32_t height = 0;
};

/// The content size of parent, after its layout is up to date.
Size content_size(lv_obj_t* parent);

/// A plain rectangle: no theme styles, no scrolling, not clickable (touches
/// go to the demo, which switches scenes). Transparent until a colour is set.
lv_obj_t* create_box(lv_obj_t* parent, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height);

/// A box filled with one colour.
lv_obj_t* create_filled_box(lv_obj_t* parent, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height,
                            lv_color_t color);

/// A label in the given font and colour, not clickable.
lv_obj_t* create_label(lv_obj_t* parent, const lv_font_t* font, lv_color_t color);

/// Fonts: large for headings and numbers on screens at least 400 pixels
/// wide, small everywhere else. Every board build enables Montserrat 14 and 20.
const lv_font_t* small_font();
const lv_font_t* large_font(std::int32_t screen_width);

/// Deterministic pseudo-random numbers (xorshift32): every run draws the
/// same scenes, so results of two runs compare.
class Random final
{
public:
    explicit Random(std::uint32_t seed) : state_(seed != 0U ? seed : 1U) {}

    std::uint32_t next();
    /// A value in [low, high].
    std::int32_t range(std::int32_t low, std::int32_t high);

private:
    std::uint32_t state_;
};

/// A point moving at a constant speed that bounces off the edges of
/// [0, max_x] x [0, max_y]. Positions are kept in 1/16 pixel, speeds are in
/// pixels per second.
class Bouncer final
{
public:
    void reset(std::int32_t x, std::int32_t y, std::int32_t speed_x, std::int32_t speed_y, std::int32_t max_x,
               std::int32_t max_y);
    void step(std::uint32_t elapsed_ms);

    std::int32_t x() const { return x_ >> kFractionBits; }
    std::int32_t y() const { return y_ >> kFractionBits; }

private:
    static constexpr int kFractionBits = 4;

    static void move_(std::int32_t& position, std::int32_t& speed, std::int32_t& remainder, std::int32_t limit,
                      std::uint32_t elapsed_ms);

    std::int32_t x_           = 0;
    std::int32_t y_           = 0;
    std::int32_t speed_x_     = 0;
    std::int32_t speed_y_     = 0;
    std::int32_t remainder_x_ = 0;  ///< sub-step movement, in 1/16 pixel x 1/1000
    std::int32_t remainder_y_ = 0;
    std::int32_t max_x_       = 0;
    std::int32_t max_y_       = 0;
};

/// Triangle wave from 0 up to peak and back within period_ms.
std::int32_t triangle_wave(std::uint32_t time_ms, std::uint32_t period_ms, std::int32_t peak);

/// Fully saturated colour around the colour wheel; phase in [0, kColorWheelSteps).
inline constexpr std::uint32_t kColorWheelSteps = 1536;
lv_color_t                     color_wheel(std::uint32_t phase);

}  // namespace benchmark_demo
