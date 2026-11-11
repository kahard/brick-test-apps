#include "MixedScene.h"

#include "SceneTools.h"

namespace benchmark_demo
{

namespace
{
    constexpr std::int32_t  kGap        = 2;
    constexpr std::int32_t  kColumns    = 2;
    constexpr std::uint32_t kBackground = 0x000000;
}  // namespace

void MixedScene::create(lv_obj_t* parent)
{
    const Size size               = content_size(parent);
    root_                         = create_filled_box(parent, 0, 0, size.width, size.height, lv_color_hex(kBackground));
    const std::int32_t tile_width = (size.width - kGap) / kColumns;
    const std::int32_t tile_height = (size.height - kGap) / kColumns;
    for (std::size_t index = 0; index < kTiles; ++index)
    {
        const auto column = static_cast<std::int32_t>(index) % kColumns;
        const auto row    = static_cast<std::int32_t>(index) / kColumns;
        lv_obj_t*  tile =
            create_box(root_, column * (tile_width + kGap), row * (tile_height + kGap), tile_width, tile_height);
        scenes_[index]->create(tile);
    }
}

void MixedScene::step(std::uint32_t elapsed_ms)
{
    if (root_ == nullptr)
        return;
    for (IScene* scene : scenes_)
        scene->step(elapsed_ms);
}

void MixedScene::destroy()
{
    // The scenes first: their objects are children of root_.
    for (IScene* scene : scenes_)
        scene->destroy();
    if (root_ != nullptr)
        lv_obj_delete(root_);
    root_ = nullptr;
}

}  // namespace benchmark_demo
