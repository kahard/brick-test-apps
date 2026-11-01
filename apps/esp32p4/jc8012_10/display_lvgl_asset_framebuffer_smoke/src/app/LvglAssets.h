#pragma once

#include "Services.h"
#include "brick/core/memory/MemoryBlock.h"
#include "lvgl.h"

namespace lvgl_asset_smoke
{
/// Both bundle images copied once into external RAM and wrapped as LVGL
/// image descriptors.
class LvglAssets final
{
public:
    LvglAssets(AssetSource& source, MemoryProvider& memory);
    Status                initialize();
    const lv_image_dsc_t* image(bool second) const { return &descriptors_[second ? 1 : 0]; }

private:
    AssetSource&                     source_;
    brick::core::memory::MemoryBlock pixels_[2];
    lv_image_dsc_t                   descriptors_[2]{};
};
}  // namespace lvgl_asset_smoke
