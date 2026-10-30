#include "LvglAssets.h"

#include "generated_assets.h"

namespace lvgl_benchmark_smoke
{
LvglAssets::LvglAssets(AssetSource& source, MemoryProvider& memory)
    : source_(source), pixels_{ brick::core::memory::MemoryBlock{ memory }, brick::core::memory::MemoryBlock{ memory } }
{
}

bool LvglAssets::initialize()
{
    constexpr generated_assets::Id ids[] = { generated_assets::Id::joy_tears, generated_assets::Id::sweat_smile };
    for (std::size_t index = 0; index < 2; ++index)
    {
        const brick::interfaces::display::AssetDescriptor* asset = generated_assets::get(ids[index]);
        if (asset == nullptr || asset->format != brick::interfaces::display::PixelFormat::rgb565
            || !pixels_[index].allocate(asset->size, brick::interfaces::memory::MemoryKind::external)
            || !source_.read(*asset, 0, pixels_[index].data(), asset->size))
            return false;
        lv_image_dsc_t& descriptor = descriptors_[index];
        descriptor.header.magic    = LV_IMAGE_HEADER_MAGIC;
        descriptor.header.cf       = LV_COLOR_FORMAT_RGB565;
        descriptor.header.w        = asset->width;
        descriptor.header.h        = asset->height;
        descriptor.header.stride   = asset->stride_bytes;
        descriptor.data_size       = asset->size;
        descriptor.data            = pixels_[index].data();
    }
    return true;
}
}  // namespace lvgl_benchmark_smoke
