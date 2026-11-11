#include "SummaryView.h"

#include "SceneTools.h"

namespace benchmark_demo
{

namespace
{
    constexpr std::int32_t  kMargin      = 8;
    constexpr std::int32_t  kCellPadding = 4;
    constexpr std::int32_t  kCellBorder  = 0;  ///< the theme draws row lines inside the padding
    constexpr std::uint32_t kColumns     = 4;
    constexpr std::int32_t  kNameShare   = 40;  ///< percent of the width for the scene name
    constexpr std::int32_t  kPercent     = 100;
    constexpr std::uint32_t kBackground  = 0x101010;
    constexpr std::uint32_t kTitleColor  = 0xFFD040;
    constexpr std::uint32_t kHintColor   = 0xA0A0A0;
    constexpr std::uint32_t kHundred     = 100;
    constexpr std::uint32_t kTen         = 10;
    constexpr std::uint32_t kThousand    = 1000;
}  // namespace

FrameStats::Result mean_result(const SceneResult* results, std::size_t count)
{
    FrameStats::Result mean;
    if (results == nullptr || count == 0)
        return mean;
    std::uint64_t fps      = 0;
    std::uint64_t frame_us = 0;
    std::uint64_t load     = 0;
    std::uint64_t measured = 0;
    for (std::size_t index = 0; index < count; ++index)
    {
        // A scene skipped before its warm-up ended has no result.
        if (results[index].result.frames == 0)
            continue;
        ++measured;
        mean.frames += results[index].result.frames;
        fps += results[index].result.fps_x100;
        frame_us += results[index].result.frame_us;
        load += results[index].result.load_pct;
    }
    if (measured == 0)
        return mean;
    mean.fps_x100 = static_cast<std::uint32_t>(fps / measured);
    mean.frame_us = static_cast<std::uint32_t>(frame_us / measured);
    mean.load_pct = static_cast<std::uint32_t>(load / measured);
    return mean;
}

void SummaryView::create(lv_obj_t* parent, const char* board, const SceneResult* results, std::size_t count,
                         const lv_font_t* font)
{
    const Size size = content_size(parent);
    root_           = create_filled_box(parent, 0, 0, size.width, size.height, lv_color_hex(kBackground));
    lv_obj_t* title = create_label(root_, font, lv_color_hex(kTitleColor));
    lv_label_set_text(title, board);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, kMargin);

    table_ = lv_table_create(root_);
    lv_obj_remove_flag(table_, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_text_font(table_, font, LV_PART_ITEMS);
    lv_obj_set_style_pad_ver(table_, fitting_padding_(size.height, static_cast<std::int32_t>(count) + 2, font),
                             LV_PART_ITEMS);
    lv_table_set_column_count(table_, kColumns);
    lv_table_set_row_count(table_, static_cast<std::uint32_t>(count) + 2U);
    const std::int32_t width      = size.width - 2 * kMargin;
    const std::int32_t name_width = width * kNameShare / kPercent;
    lv_table_set_column_width(table_, 0, name_width);
    for (std::uint32_t column = 1; column < kColumns; ++column)
        lv_table_set_column_width(table_, column, (width - name_width) / static_cast<std::int32_t>(kColumns - 1));
    lv_table_set_cell_value(table_, 0, 0, "Scene");
    lv_table_set_cell_value(table_, 0, 1, "FPS");
    lv_table_set_cell_value(table_, 0, 2, "ms");
    lv_table_set_cell_value(table_, 0, 3, "CPU");
    for (std::size_t index = 0; index < count; ++index)
        fill_row_(static_cast<std::uint32_t>(index) + 1U, results[index].name, results[index].result);
    fill_row_(static_cast<std::uint32_t>(count) + 1U, "Mean", mean_result(results, count));
    lv_obj_align_to(table_, title, LV_ALIGN_OUT_BOTTOM_MID, 0, kMargin);

    lv_obj_t* hint = create_label(root_, small_font(), lv_color_hex(kHintColor));
    lv_label_set_text_static(hint, "Tap: next round");
    lv_obj_align(hint, LV_ALIGN_BOTTOM_MID, 0, -kMargin);
}

std::int32_t SummaryView::fitting_padding_(std::int32_t height, std::int32_t rows, const lv_font_t* font)
{
    // Between the board name at the top and the hint at the bottom; on a
    // small screen (CYD 320x240) the rows get less padding so all of them fit.
    const std::int32_t line    = lv_font_get_line_height(font);
    const std::int32_t room    = height - (line + 3 * kMargin) - (lv_font_get_line_height(small_font()) + kMargin);
    std::int32_t       padding = kCellPadding;
    while (padding > 0 && rows * (line + 2 * padding + kCellBorder) > room)
        --padding;
    return padding;
}

void SummaryView::fill_row_(std::uint32_t row, const char* name, const FrameStats::Result& result)
{
    lv_table_set_cell_value(table_, row, 0, name);
    lv_table_set_cell_value_fmt(table_, row, 1, "%u.%u", static_cast<unsigned>(result.fps_x100 / kHundred),
                                static_cast<unsigned>(result.fps_x100 % kHundred / kTen));
    lv_table_set_cell_value_fmt(table_, row, 2, "%u.%u", static_cast<unsigned>(result.frame_us / kThousand),
                                static_cast<unsigned>(result.frame_us % kThousand / kHundred));
    lv_table_set_cell_value_fmt(table_, row, 3, "%u%%", static_cast<unsigned>(result.load_pct));
}

void SummaryView::destroy()
{
    if (root_ != nullptr)
        lv_obj_delete(root_);
    root_  = nullptr;
    table_ = nullptr;
}

}  // namespace benchmark_demo
