#include "RectanglesScene.h"

namespace benchmark_demo
{

namespace
{
    constexpr std::uint32_t kSeed         = 0x5EED1234;
    constexpr std::int32_t  kMinSpeed     = 40;  ///< pixels per second
    constexpr std::int32_t  kMaxSpeed     = 180;
    constexpr std::int32_t  kMinFraction  = 10;  ///< sides between 1/10 ...
    constexpr std::int32_t  kMaxFraction  = 4;   ///< ... and 1/4 of the area
    constexpr std::int32_t  kRadius       = 12;
    constexpr std::size_t   kRoundedEvery = 2;  ///< every second rectangle has rounded corners
    constexpr std::size_t   kBlendedEvery = 3;  ///< every third one is half transparent
    constexpr std::uint32_t kBackground   = 0x202020;

    std::int32_t signed_speed(Random& random)
    {
        const std::int32_t speed = random.range(kMinSpeed, kMaxSpeed);
        return (random.next() & 1U) != 0U ? speed : -speed;
    }
}  // namespace

void RectanglesScene::create(lv_obj_t* parent)
{
    const Size size = content_size(parent);
    root_           = create_filled_box(parent, 0, 0, size.width, size.height, lv_color_hex(kBackground));
    Random random(kSeed);
    for (std::size_t index = 0; index < kRectangles; ++index)
    {
        const std::int32_t width  = random.range(size.width / kMinFraction, size.width / kMaxFraction);
        const std::int32_t height = random.range(size.height / kMinFraction, size.height / kMaxFraction);
        const lv_color_t   color  = color_wheel(random.next() % kColorWheelSteps);
        boxes_[index]             = create_filled_box(root_, 0, 0, width, height, color);
        if (index % kRoundedEvery == 0)
            lv_obj_set_style_radius(boxes_[index], kRadius, 0);
        if (index % kBlendedEvery == 0)
            lv_obj_set_style_bg_opa(boxes_[index], LV_OPA_50, 0);
        const std::int32_t max_x = size.width - width;
        const std::int32_t max_y = size.height - height;
        motion_[index].reset(random.range(0, max_x), random.range(0, max_y), signed_speed(random), signed_speed(random),
                             max_x, max_y);
        lv_obj_set_pos(boxes_[index], motion_[index].x(), motion_[index].y());
    }
}

void RectanglesScene::step(std::uint32_t elapsed_ms)
{
    if (root_ == nullptr)
        return;
    for (std::size_t index = 0; index < kRectangles; ++index)
    {
        motion_[index].step(elapsed_ms);
        lv_obj_set_pos(boxes_[index], motion_[index].x(), motion_[index].y());
    }
}

void RectanglesScene::destroy()
{
    if (root_ != nullptr)
        lv_obj_delete(root_);
    root_  = nullptr;
    boxes_ = {};
}

}  // namespace benchmark_demo
