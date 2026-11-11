#include "TextScene.h"

#include "SceneTools.h"

namespace benchmark_demo
{

namespace
{
    constexpr std::int32_t  kMargin        = 8;
    constexpr std::int32_t  kRowHeight     = 22;
    constexpr std::int32_t  kClockHeight   = 36;
    constexpr std::int32_t  kWideMinWidth  = 360;
    constexpr std::uint32_t kCounterModulo = 1000000;
    constexpr std::uint32_t kMinuteMs      = 60000;
    constexpr std::uint32_t kSecondMs      = 1000;
    constexpr std::uint32_t kBackground    = 0x181018;
    constexpr std::uint32_t kTextColor     = 0xE0E0FF;
    constexpr std::uint32_t kClockColor    = 0xFFD040;
}  // namespace

void TextScene::create(lv_obj_t* parent)
{
    const Size size = content_size(parent);
    root_           = create_filled_box(parent, 0, 0, size.width, size.height, lv_color_hex(kBackground));
    time_ms_        = 0;
    counter_        = 0;

    clock_ = create_label(root_, large_font(size.width), lv_color_hex(kClockColor));
    lv_obj_align(clock_, LV_ALIGN_TOP_MID, 0, kMargin);

    const std::size_t  columns   = size.width >= kWideMinWidth ? kMaxColumns : kMaxColumns - 1;
    const std::int32_t free_rows = (size.height - kClockHeight - kMargin) / kRowHeight;
    const std::size_t  rows =
        free_rows <= 0 ?
             0 :
             (static_cast<std::size_t>(free_rows) < kMaxRows ? static_cast<std::size_t>(free_rows) : kMaxRows);
    const std::int32_t column_width = (size.width - kMargin) / static_cast<std::int32_t>(columns);
    count_                          = rows * columns;
    for (std::size_t index = 0; index < count_; ++index)
    {
        labels_[index] = create_label(root_, small_font(), lv_color_hex(kTextColor));
        lv_obj_set_pos(labels_[index], kMargin + static_cast<std::int32_t>(index % columns) * column_width,
                       kClockHeight + kMargin + static_cast<std::int32_t>(index / columns) * kRowHeight);
    }
}

void TextScene::step(std::uint32_t elapsed_ms)
{
    if (root_ == nullptr)
        return;
    time_ms_ += elapsed_ms;
    ++counter_;
    lv_label_set_text_fmt(clock_, "%02u:%02u.%03u", static_cast<unsigned>(time_ms_ / kMinuteMs),
                          static_cast<unsigned>(time_ms_ % kMinuteMs / kSecondMs),
                          static_cast<unsigned>(time_ms_ % kSecondMs));
    for (std::size_t index = 0; index < count_; ++index)
    {
        // Every label counts at its own pace, so each frame changes all digits.
        const std::uint32_t value = counter_ * static_cast<std::uint32_t>(2 * index + 7) % kCounterModulo;
        lv_label_set_text_fmt(labels_[index], "#%02u %06u", static_cast<unsigned>(index), static_cast<unsigned>(value));
    }
}

void TextScene::destroy()
{
    if (root_ != nullptr)
        lv_obj_delete(root_);
    root_   = nullptr;
    clock_  = nullptr;
    labels_ = {};
    count_  = 0;
}

}  // namespace benchmark_demo
