#include "AssetSources.h"

#include <cstdio>
#include <cstring>

#include "generated_assets.h"

namespace cyd_asset_stream_smoke
{
Status AssetSources::RamSource::read(const brick::interfaces::display::AssetDescriptor& asset, std::size_t offset,
                                     std::uint8_t* destination, std::size_t bytes)
{
    if (data_ == nullptr || destination == nullptr || offset > asset.size || bytes > asset.size - offset
        || asset.offset > generated_assets::bundle_size || asset.size > generated_assets::bundle_size - asset.offset)
        return Status::error;
    std::memcpy(destination, data_ + asset.offset + offset, bytes);
    return Status::ok;
}

Status AssetSources::SdSource::begin()
{
    close();
    if (const Status status = filesystem_.open("/sdcard/ASSETS.BIN", "rb", file_); !is_ok(status))
        return status;
    // The S3 and LVGL bundles have different sizes. Reject them before
    // displaying even the first frame; this demo requires its own bundle.
    std::uint8_t byte = 0;
    if (!is_ok(file_->seek(static_cast<long>(generated_assets::bundle_size - 1), SEEK_SET))
        || file_->read(&byte, 1, 1) != 1 || file_->read(&byte, 1, 1) != 0)
    {
        close();
        return Status::error;
    }
    return Status::ok;
}

Status AssetSources::SdSource::read(const brick::interfaces::display::AssetDescriptor& asset, std::size_t offset,
                                    std::uint8_t* destination, std::size_t bytes)
{
    if (!file_)
        return Status::not_ready;
    if (destination == nullptr || offset > asset.size || bytes > asset.size - offset)
        return Status::invalid_argument;
    if (const Status status = file_->seek(static_cast<long>(asset.offset + offset), SEEK_SET); !is_ok(status))
        return status;
    return file_->read(destination, 1, bytes) == bytes ? Status::ok : Status::error;
}

AssetSources::AssetSources(AssetSource& flash, RemovableFileSystem& sd, MemoryProvider& memory)
    : flash_(flash), sd_(sd), sd_source_(sd), repository_(&flash, &ram_source_, &sd_source_), ram_bundle_(memory)
{
}

Status AssetSources::prepare_ram_copy()
{
    if (ram_ready_)
        return Status::ok;
    if (!is_ok(ram_bundle_.allocate(generated_assets::bundle_size, brick::interfaces::memory::MemoryKind::external)))
        return Status::error;
    for (std::size_t index = 0; index < generated_assets::entry_count; ++index)
    {
        const brick::interfaces::display::AssetDescriptor& asset = generated_assets::entries[index];
        if (!is_ok(flash_.read(asset, 0, ram_bundle_.data() + asset.offset, asset.size)))
            return Status::error;
    }
    ram_source_.set_data(ram_bundle_.data());
    ram_ready_ = true;
    return Status::ok;
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
    if (storage == 1 && !is_ok(prepare_ram_copy()))
        return nullptr;
    if (storage == 2 && !sd_ready_)
    {
        if (!is_ok(sd_.mount()) || !is_ok(sd_source_.begin()))
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
