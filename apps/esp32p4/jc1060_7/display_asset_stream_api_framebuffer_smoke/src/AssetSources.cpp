#include "AssetSources.h"
#include "PixelMemory.h"
#include "brick/core/image/AssetRepository.h"
#include "brick/platform/esp32/PartitionAssetSource.h"
#include "brick/platform/esp32/p4/SdmmcFileSystem.h"
#include "generated_assets.h"
#include <cstring>

namespace
{
class RamAssetSource final : public brick::interfaces::display::IAssetSource
{
public:
    void set_data(std::uint8_t* data) { data_ = data; }
    bool read(const brick::interfaces::display::AssetDescriptor& asset, std::size_t offset, std::uint8_t* destination,
              std::size_t bytes) override
    {
        if (data_ == nullptr || destination == nullptr || offset > asset.size || bytes > asset.size - offset
            || asset.offset > generated_assets::bundle_size
            || asset.size > generated_assets::bundle_size - asset.offset)
            return false;
        std::memcpy(destination, data_ + asset.offset + offset, bytes);
        return true;
    }

private:
    std::uint8_t* data_ = nullptr;
};

class SdAssetSource final : public brick::interfaces::display::IAssetSource
{
public:
    explicit SdAssetSource(brick::interfaces::storage::IFileSystem& filesystem) : filesystem_(filesystem) {}
    bool begin()
    {
        close();
        file_ = filesystem_.open("/sdcard/ASSETS.BIN", "rb");
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
    void close() { file_.reset(); }
    bool read(const brick::interfaces::display::AssetDescriptor& asset, std::size_t offset, std::uint8_t* destination,
              std::size_t bytes) override
    {
        if (!file_ || destination == nullptr || offset > asset.size || bytes > asset.size - offset
            || !file_->seek(static_cast<long>(asset.offset + offset), SEEK_SET))
            return false;
        return file_->read(destination, 1, bytes) == bytes;
    }

private:
    brick::interfaces::storage::IFileSystem&           filesystem_;
    std::unique_ptr<brick::interfaces::storage::IFile> file_;
};
}  // namespace

struct AssetSources::Impl
{
    explicit Impl(brick::platform::esp32::SdmmcFileSystem& card)
        : sd(card), sd_source(card), repository(&flash_source, &psram_source, &sd_source)
    {
    }

    bool prepare_psram()
    {
        if (psram_ready)
            return true;
        if (!psram_bundle.allocate(generated_assets::bundle_size))
            return false;
        for (std::size_t index = 0; index < generated_assets::entry_count; ++index)
        {
            const brick::interfaces::display::AssetDescriptor& asset = generated_assets::entries[index];
            if (!flash_source.read(asset, 0, psram_bundle.data() + asset.offset, asset.size))
                return false;
        }
        psram_source.set_data(psram_bundle.data());
        psram_ready = true;
        return true;
    }
    void close_sd()
    {
        // Close the file before tearing down the filesystem.
        sd_source.close();
        sd.unmount();
        sd_ready = false;
    }

    brick::platform::esp32::SdmmcFileSystem&     sd;
    brick::platform::esp32::PartitionAssetSource flash_source{ "assets" };
    RamAssetSource                               psram_source;
    SdAssetSource                                sd_source;
    brick::core::image::AssetRepository          repository;
    PixelMemory                                  psram_bundle;
    bool                                         psram_ready = false;
    bool                                         sd_ready    = false;
};

AssetSources::AssetSources(brick::platform::esp32::SdmmcFileSystem& card) : impl_(std::make_unique<Impl>(card))
{
}
AssetSources::~AssetSources() = default;
bool AssetSources::initialize()
{
    return impl_->flash_source.begin();
}
brick::interfaces::display::IAssetSource& AssetSources::flash()
{
    return impl_->flash_source;
}

brick::interfaces::display::IAssetSource* AssetSources::select(std::uint8_t storage)
{
    if (storage > 2)
        return nullptr;
    if (storage != 2 && impl_->sd_ready)
        impl_->close_sd();
    if (storage == 1 && !impl_->prepare_psram())
        return nullptr;
    if (storage == 2 && !impl_->sd_ready)
    {
        if (!impl_->sd.mount() || !impl_->sd_source.begin())
        {
            impl_->close_sd();
            return nullptr;
        }
        impl_->sd_ready = true;
    }
    using brick::interfaces::display::AssetStorage;
    impl_->repository.set_storage(storage == 0 ? AssetStorage::flash_partition :
                                  storage == 1 ? AssetStorage::psram_cache :
                                                 AssetStorage::usb_or_sd);
    return impl_->repository.source();
}

void AssetSources::fallback_to_flash()
{
    impl_->close_sd();
    impl_->repository.set_storage(brick::interfaces::display::AssetStorage::flash_partition);
}
