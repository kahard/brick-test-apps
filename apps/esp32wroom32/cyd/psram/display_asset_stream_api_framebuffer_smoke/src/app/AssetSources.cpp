#include "AssetSources.h"

#include <cstdio>
#include <cstring>

#include "generated_assets.h"

namespace cyd_asset_stream_smoke
{
bool AssetSources::RamSource::read(const brick::interfaces::display::AssetDescriptor& asset, std::size_t offset,
                                   std::uint8_t* destination, std::size_t bytes)
{
    if (data_ == nullptr || destination == nullptr || offset > asset.size || bytes > asset.size - offset
        || asset.offset > generated_assets::bundle_size || asset.size > generated_assets::bundle_size - asset.offset)
        return false;
    std::memcpy(destination, data_ + asset.offset + offset, bytes);
    return true;
}

bool AssetSources::SdSource::begin()
{
    close();
    filesystem_.open("/sdcard/ASSETS.BIN", "rb", file_);
    // The S3 and LVGL bundles have different sizes. Reject them before
    // displaying even the first frame; this demo requires its own bundle.
    std::uint8_t byte = 0;
    if (!file_ || !file_->seek(static_cast<long>(generated_assets::bundle_size - 1), SEEK_SET)
        || file_->read(&byte, 1, 1) != 1 || file_->read(&byte, 1, 1) != 0)
    {
        close();
        return false;
    }
    return true;
}

bool AssetSources::SdSource::read(const brick::interfaces::display::AssetDescriptor& asset, std::size_t offset,
                                  std::uint8_t* destination, std::size_t bytes)
{
    if (!file_ || destination == nullptr || offset > asset.size || bytes > asset.size - offset
        || !file_->seek(static_cast<long>(asset.offset + offset), SEEK_SET))
        return false;
    return file_->read(destination, 1, bytes) == bytes;
}

AssetSources::AssetSources(AssetSource& flash, RemovableFileSystem& sd, MemoryProvider& memory)
    : flash_(flash), sd_(sd), sd_source_(sd), repository_(&flash, &ram_source_, &sd_source_), ram_bundle_(memory)
{
}

bool AssetSources::prepare_ram_copy()
{
    if (ram_ready_)
        return true;
    if (!ram_bundle_.allocate(generated_assets::bundle_size, brick::interfaces::memory::MemoryKind::external))
        return false;
    for (std::size_t index = 0; index < generated_assets::entry_count; ++index)
    {
        const brick::interfaces::display::AssetDescriptor& asset = generated_assets::entries[index];
        if (!flash_.read(asset, 0, ram_bundle_.data() + asset.offset, asset.size))
            return false;
    }
    ram_source_.set_data(ram_bundle_.data());
    ram_ready_ = true;
    return true;
}

void AssetSources::close_sd()
{
    // Close the file before tearing down the filesystem.
    sd_source_.close();
    sd_.unmount();
    sd_ready_ = false;
}

AssetSource* AssetSources::select(std::uint8_t storage)
{
    if (storage > 2)
        return nullptr;
    if (storage != 2 && sd_ready_)
        close_sd();
    if (storage == 1 && !prepare_ram_copy())
        return nullptr;
    if (storage == 2 && !sd_ready_)
    {
        if (!sd_.mount() || !sd_source_.begin())
        {
            close_sd();
            return nullptr;
        }
        sd_ready_ = true;
    }
    using brick::interfaces::display::AssetStorage;
    repository_.set_storage(storage == 0 ? AssetStorage::flash_partition :
                            storage == 1 ? AssetStorage::psram_cache :
                                           AssetStorage::usb_or_sd);
    return repository_.source();
}

void AssetSources::fallback_to_flash()
{
    close_sd();
    repository_.set_storage(brick::interfaces::display::AssetStorage::flash_partition);
}
}  // namespace cyd_asset_stream_smoke
