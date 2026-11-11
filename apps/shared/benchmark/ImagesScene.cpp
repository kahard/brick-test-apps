#include "ImagesScene.h"

namespace benchmark_demo
{

namespace
{
    constexpr std::uint32_t kSeed       = 0x1A2B3C4D;
    constexpr std::int32_t  kMinSpeed   = 60;  ///< pixels per second
    constexpr std::int32_t  kMaxSpeed   = 220;
    constexpr std::uint32_t kBackground = 0x0C1830;

    std::int32_t signed_speed(Random& random)
    {
        const std::int32_t speed = random.range(kMinSpeed, kMaxSpeed);
        return (random.next() & 1U) != 0U ? speed : -speed;
    }
}  // namespace

void ImagesScene::create(lv_obj_t* parent)
{
    const Size size          = content_size(parent);
    root_                    = create_filled_box(parent, 0, 0, size.width, size.height, lv_color_hex(kBackground));
    const std::int32_t max_x = size.width - sprite_.diameter();
    const std::int32_t max_y = size.height - sprite_.diameter();
    Random             random(kSeed);
    for (std::size_t index = 0; index < kSprites; ++index)
    {
        images_[index] = lv_image_create(root_);
        lv_obj_remove_flag(images_[index], LV_OBJ_FLAG_CLICKABLE);
        lv_image_set_src(images_[index], sprite_.descriptor());
        motion_[index].reset(random.range(0, max_x), random.range(0, max_y), signed_speed(random), signed_speed(random),
                             max_x, max_y);
        lv_obj_set_pos(images_[index], motion_[index].x(), motion_[index].y());
    }
}

void ImagesScene::step(std::uint32_t elapsed_ms)
{
    if (root_ == nullptr)
        return;
    for (std::size_t index = 0; index < kSprites; ++index)
    {
        motion_[index].step(elapsed_ms);
        lv_obj_set_pos(images_[index], motion_[index].x(), motion_[index].y());
    }
}

void ImagesScene::destroy()
{
    if (root_ != nullptr)
        lv_obj_delete(root_);
    root_   = nullptr;
    images_ = {};
}

}  // namespace benchmark_demo
