#include "WidgetsScene.h"

#include "SceneTools.h"

namespace benchmark_demo
{

namespace
{
    constexpr std::int32_t  kPercent           = 100;
    constexpr std::int32_t  kMargin            = 8;
    constexpr std::int32_t  kBarHeight         = 14;
    constexpr std::uint32_t kChartPoints       = 32;
    constexpr std::uint32_t kSpinnerPeriodMs   = 1000;
    constexpr std::uint32_t kSpinnerArcDegrees = 270;
    /// Periods of the moving values, chosen so they never line up.
    constexpr std::uint32_t kArcPeriodsMs[]   = { 2300, 3100 };
    constexpr std::uint32_t kBarPeriodsMs[]   = { 1700, 2900, 4100 };
    constexpr std::uint32_t kSliderPeriodMs   = 3700;
    constexpr std::uint32_t kChartPeriodsMs[] = { 1300, 5300 };
    constexpr std::uint32_t kBackground       = 0x101828;
}  // namespace

void WidgetsScene::create(lv_obj_t* parent)
{
    const Size size = content_size(parent);
    root_           = create_filled_box(parent, 0, 0, size.width, size.height, lv_color_hex(kBackground));
    time_ms_        = 0;
    create_top_row_(size.width, size.height / 2);
    create_bottom_row_(size.height / 2, size.width, size.height - size.height / 2);
}

void WidgetsScene::create_top_row_(std::int32_t width, std::int32_t height)
{
    // Arc, spinner, arc in three equal cells.
    const std::int32_t cell     = width / 3;
    const std::int32_t diameter = (cell < height ? cell : height) - 2 * kMargin;
    for (std::size_t index = 0; index < kArcs; ++index)
    {
        arcs_[index] = lv_arc_create(root_);
        lv_obj_remove_flag(arcs_[index], LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_size(arcs_[index], diameter, diameter);
        lv_obj_set_pos(arcs_[index], static_cast<std::int32_t>(index) * 2 * cell + (cell - diameter) / 2,
                       (height - diameter) / 2);
    }
    lv_obj_t* spinner = lv_spinner_create(root_);
    lv_spinner_set_anim_params(spinner, kSpinnerPeriodMs, kSpinnerArcDegrees);
    lv_obj_set_size(spinner, diameter, diameter);
    lv_obj_set_pos(spinner, cell + (cell - diameter) / 2, (height - diameter) / 2);
}

void WidgetsScene::create_bottom_row_(std::int32_t top, std::int32_t width, std::int32_t height)
{
    // Bars and a slider on the left, the chart on the right.
    const std::int32_t column = width / 2;
    const std::int32_t rows   = static_cast<std::int32_t>(kBars) + 1;
    const std::int32_t pitch  = height / rows;
    for (std::size_t index = 0; index < kBars; ++index)
    {
        bars_[index] = lv_bar_create(root_);
        lv_obj_set_size(bars_[index], column - 2 * kMargin, kBarHeight);
        lv_obj_set_pos(bars_[index], kMargin,
                       top + static_cast<std::int32_t>(index) * pitch + (pitch - kBarHeight) / 2);
    }
    slider_ = lv_slider_create(root_);
    lv_obj_remove_flag(slider_, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(slider_, column - 4 * kMargin, kBarHeight);
    lv_obj_set_pos(slider_, 2 * kMargin, top + static_cast<std::int32_t>(kBars) * pitch + (pitch - kBarHeight) / 2);

    chart_ = lv_chart_create(root_);
    lv_obj_remove_flag(chart_, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(chart_, column - 2 * kMargin, height - 2 * kMargin);
    lv_obj_set_pos(chart_, column + kMargin, top + kMargin);
    lv_chart_set_point_count(chart_, kChartPoints);
    lv_chart_set_axis_range(chart_, LV_CHART_AXIS_PRIMARY_Y, 0, kPercent);
    series_ = lv_chart_add_series(chart_, lv_palette_main(LV_PALETTE_CYAN), LV_CHART_AXIS_PRIMARY_Y);
}

void WidgetsScene::step(std::uint32_t elapsed_ms)
{
    if (root_ == nullptr)
        return;
    time_ms_ += elapsed_ms;
    for (std::size_t index = 0; index < kArcs; ++index)
        lv_arc_set_value(arcs_[index], triangle_wave(time_ms_, kArcPeriodsMs[index], kPercent));
    for (std::size_t index = 0; index < kBars; ++index)
        lv_bar_set_value(bars_[index], triangle_wave(time_ms_, kBarPeriodsMs[index], kPercent), LV_ANIM_OFF);
    lv_slider_set_value(slider_, triangle_wave(time_ms_, kSliderPeriodMs, kPercent), LV_ANIM_OFF);
    const std::int32_t sample =
        (triangle_wave(time_ms_, kChartPeriodsMs[0], kPercent) + triangle_wave(time_ms_, kChartPeriodsMs[1], kPercent))
        / 2;
    lv_chart_set_next_value(chart_, series_, sample);
}

void WidgetsScene::destroy()
{
    if (root_ != nullptr)
        lv_obj_delete(root_);
    root_   = nullptr;
    arcs_   = {};
    bars_   = {};
    slider_ = nullptr;
    chart_  = nullptr;
    series_ = nullptr;
}

}  // namespace benchmark_demo
