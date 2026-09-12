#include "BenchmarkSeries.h"

BenchmarkSeries::BenchmarkSeries(brick::interfaces::time::ITimeProvider& time,
                                 brick::interfaces::logging::ILogger&    logger)
    : time_(time), logger_(logger)
{
}

void BenchmarkSeries::initialize(lv_display_t* display, LvglAssets& assets)
{
    display_ = display;
    assets_  = &assets;
    image_   = lv_image_create(lv_screen_active());
    lv_image_set_src(image_, assets_->image(false));
    lv_obj_center(image_);
    next_series_ = time_.millis() + 1000;
}

void BenchmarkSeries::update()
{
    if (static_cast<std::int32_t>(time_.millis() - next_series_) < 0)
        return;
    if (frames_ == 0)
    {
        if (colors_)
            lv_obj_add_flag(image_, LV_OBJ_FLAG_HIDDEN);
        else
        {
            lv_obj_remove_flag(image_, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x202040), 0);
        }
        started_ = time_.micros();
    }
    if (colors_)
        lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex((frames_ & 1U) ? 0x0000FF : 0xFF0000), 0);
    else
        lv_image_set_src(image_, assets_->image((frames_ & 1U) != 0));
    lv_refr_now(display_);
    if (++frames_ < 60)
        return;
    const std::uint64_t elapsed = time_.micros() - started_;
    logger_.info("lvgl_benchmark", "%s FULL frames=%u elapsed=%lluus fps=%.2f", colors_ ? "colors" : "preloaded assets",
                 static_cast<unsigned>(frames_), static_cast<unsigned long long>(elapsed),
                 elapsed ? frames_ * 1000000.0 / elapsed : 0.0);
    frames_      = 0;
    colors_      = !colors_;
    next_series_ = time_.millis() + 2000;
}
