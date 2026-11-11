#include "FullScreenScene.h"

#include "SceneTools.h"

namespace benchmark_demo
{

namespace
{
    /// A full turn of the colour wheel takes four seconds.
    constexpr std::uint32_t kPhasePerSecond = kColorWheelSteps / 4;
    constexpr std::uint32_t kMsPerSecond    = 1000;
    constexpr std::uint32_t kTextColor      = 0xFFFFFF;
}  // namespace

void FullScreenScene::create(lv_obj_t* parent)
{
    const Size size = content_size(parent);
    root_           = create_filled_box(parent, 0, 0, size.width, size.height, color_wheel(0));
    lv_obj_set_style_bg_grad_dir(root_, LV_GRAD_DIR_VER, 0);
    lv_obj_set_style_bg_grad_color(root_, color_wheel(kColorWheelSteps / 2), 0);
    label_ = create_label(root_, large_font(size.width), lv_color_hex(kTextColor));
    lv_obj_center(label_);
    phase_  = 0;
    frames_ = 0;
}

void FullScreenScene::step(std::uint32_t elapsed_ms)
{
    if (root_ == nullptr)
        return;
    phase_ = (phase_ + elapsed_ms * kPhasePerSecond / kMsPerSecond + 1U) % kColorWheelSteps;
    ++frames_;
    lv_obj_set_style_bg_color(root_, color_wheel(phase_), 0);
    lv_obj_set_style_bg_grad_color(root_, color_wheel(phase_ + kColorWheelSteps / 2), 0);
    lv_label_set_text_fmt(label_, "frame %u", static_cast<unsigned>(frames_));
}

void FullScreenScene::destroy()
{
    if (root_ != nullptr)
        lv_obj_delete(root_);
    root_  = nullptr;
    label_ = nullptr;
}

}  // namespace benchmark_demo
