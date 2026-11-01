#include "LvglAssets.h"

#include "generated_assets.h"

namespace lvgl_asset_smoke
{
LvglAssets::LvglAssets(AssetSource& source, MemoryProvider& memory)
    : source_(source), pixels_{ brick::core::memory::MemoryBlock{ memory }, brick::core::memory::MemoryBlock{ memory } }
{
}

Status LvglAssets::initialize()
{
    constexpr generated_assets::Id ids[] = { generated_assets::Id::joy_tears, generated_assets::Id::sweat_smile };
    for (std::size_t index = 0; index < 2; ++index)
    {
        const brick::interfaces::display::AssetDescriptor* asset = generated_assets::get(ids[index]);
        if (asset == nullptr)
            return Status::not_found;
        if (asset->format != brick::interfaces::display::PixelFormat::rgb565)
            return Status::not_supported;
        if (const Status status = pixels_[index].allocate(asset->size, brick::interfaces::memory::MemoryKind::external);
            !is_ok(status))
            return status;
        if (const Status status = source_.read(*asset, 0, pixels_[index].data(), asset->size); !is_ok(status))
            return status;
        lv_image_dsc_t& descriptor = descriptors_[index];
        descriptor.header.magic    = LV_IMAGE_HEADER_MAGIC;
        descriptor.header.cf       = LV_COLOR_FORMAT_RGB565;
        descriptor.header.w        = asset->width;
        descriptor.header.h        = asset->height;
        descriptor.header.stride   = asset->stride_bytes;
        descriptor.data_size       = asset->size;
        descriptor.data            = pixels_[index].data();
    }
    return Status::ok;
}
}  // namespace lvgl_asset_smoke
