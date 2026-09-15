#include "AssetSources.h"

#include <cstdio>
#include <cstring>

#include "brick/core/image/AssetRepository.h"
#include "brick/platform/esp32/PartitionAssetSource.h"
#include "esp_heap_caps.h"
#include "generated_assets.h"

namespace cyd_asset_stream_smoke
{
namespace
{
    class PsramAssetSource final : public brick::interfaces::display::IAssetSource
    {
    public:
        void set_data(std::uint8_t* data) { data_ = data; }

        bool read(const brick::interfaces::display::AssetDescriptor& asset, std::size_t offset,
                  std::uint8_t* destination, std::size_t bytes) override
        {
            if (data_ == nullptr || destination == nullptr || offset > asset.size || bytes > asset.size - offset)
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

        bool read(const brick::interfaces::display::AssetDescriptor& asset, std::size_t offset,
                  std::uint8_t* destination, std::size_t bytes) override
        {
            if (destination == nullptr || offset > asset.size || bytes > asset.size - offset)
                return false;

            std::unique_ptr<brick::interfaces::storage::IFile> file = filesystem_.open("/sdcard/ASSETS.BIN", "rb");
            if (!file || !file->seek(static_cast<long>(asset.offset + offset), SEEK_SET))
                return false;
            return file->read(destination, 1, bytes) == bytes;
        }

    private:
        brick::interfaces::storage::IFileSystem& filesystem_;
    };
}  // namespace

struct AssetSources::Impl
{
    explicit Impl(brick::interfaces::storage::IFileSystem& filesystem)
        : filesystem(filesystem), flash_source("assets"), sd_source(filesystem),
          repository(&flash_source, &psram_source, &sd_source)
    {
    }

    ~Impl() { heap_caps_free(psram_bundle); }

    bool prepare_psram()
    {
        if (psram_ready)
            return true;

        if (psram_bundle == nullptr)
        {
            psram_bundle = static_cast<std::uint8_t*>(
                heap_caps_malloc(generated_assets::bundle_size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
            if (psram_bundle == nullptr)
                return false;
            psram_source.set_data(psram_bundle);
        }

        for (std::size_t index = 0; index < generated_assets::entry_count; ++index)
        {
            const brick::interfaces::display::AssetDescriptor& asset = generated_assets::entries[index];
            if (!flash_source.read(asset, 0, psram_bundle + asset.offset, asset.size))
                return false;
        }
        psram_ready = true;
        return true;
    }

    brick::interfaces::storage::IFileSystem&     filesystem;
    brick::platform::esp32::PartitionAssetSource flash_source;
    PsramAssetSource                             psram_source;
    SdAssetSource                                sd_source;
    brick::core::image::AssetRepository          repository;
    std::uint8_t*                                psram_bundle = nullptr;
    bool                                         psram_ready  = false;
};

AssetSources::AssetSources(brick::interfaces::storage::IFileSystem& filesystem) : impl_(std::make_unique<Impl>(filesystem))
{
}
AssetSources::~AssetSources() = default;

bool AssetSources::initialize()
{
    return impl_->flash_source.begin();
}

brick::interfaces::display::IAssetSource* AssetSources::select(std::uint8_t storage)
{
    if (storage == 1U && !impl_->prepare_psram())
        return nullptr;
    if (storage == 2U && !impl_->filesystem.mounted() && !impl_->filesystem.mount())
        return nullptr;

    const brick::interfaces::display::AssetStorage selected =
        storage == 0U ? brick::interfaces::display::AssetStorage::flash_partition :
        storage == 1U ? brick::interfaces::display::AssetStorage::psram_cache :
                        brick::interfaces::display::AssetStorage::usb_or_sd;
    impl_->repository.set_storage(selected);
    return impl_->repository.source();
}

brick::interfaces::display::IAssetSource& AssetSources::flash()
{
    return impl_->flash_source;
}

void AssetSources::fallback_to_flash()
{
    impl_->filesystem.unmount();
    impl_->repository.set_storage(brick::interfaces::display::AssetStorage::flash_partition);
}
}  // namespace cyd_asset_stream_smoke
