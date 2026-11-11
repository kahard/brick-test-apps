#include "Hud.h"

#include "SceneTools.h"

namespace benchmark_demo
{

namespace
{
    constexpr std::int32_t  kPadding     = 6;
    constexpr std::uint32_t kBackground  = 0x303848;
    constexpr std::uint32_t kTitleColor  = 0xFFFFFF;
    constexpr std::uint32_t kNumberColor = 0x80FF80;
    constexpr std::uint32_t kHundred     = 100;
    constexpr std::uint32_t kTen         = 10;
    constexpr std::uint32_t kThousand    = 1000;
}  // namespace

void Hud::create(lv_obj_t* screen, std::int32_t width, std::int32_t height, const lv_font_t* font)
{
    lv_obj_t* bar = create_filled_box(screen, 0, 0, width, height, lv_color_hex(kBackground));
    title_        = create_label(bar, font, lv_color_hex(kTitleColor));
    lv_obj_align(title_, LV_ALIGN_LEFT_MID, kPadding, 0);
    numbers_ = create_label(bar, font, lv_color_hex(kNumberColor));
    lv_obj_align(numbers_, LV_ALIGN_RIGHT_MID, -kPadding, 0);
    lv_label_set_text_static(numbers_, "");
}

void Hud::show_scene(std::size_t number, std::size_t count, const char* name)
{
    lv_label_set_text_fmt(title_, "%u/%u %s", static_cast<unsigned>(number), static_cast<unsigned>(count), name);
}

void Hud::show_title(const char* title)
{
    lv_label_set_text(title_, title);
}

void Hud::show_result(const FrameStats::Result& result)
{
    lv_label_set_text_fmt(
        numbers_, "%u.%u fps  %u.%u ms  CPU %u%%", static_cast<unsigned>(result.fps_x100 / kHundred),
        static_cast<unsigned>(result.fps_x100 % kHundred / kTen), static_cast<unsigned>(result.frame_us / kThousand),
        static_cast<unsigned>(result.frame_us % kThousand / kHundred), static_cast<unsigned>(result.load_pct));
}

}  // namespace benchmark_demo
