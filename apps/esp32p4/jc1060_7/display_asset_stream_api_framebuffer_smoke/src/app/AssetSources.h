#pragma once

#include <cstddef>
#include <cstdint>

#include "Services.h"
#include "brick/core/image/AssetRepository.h"
#include "brick/core/memory/MemoryBlock.h"

namespace asset_stream_smoke
{
/// The same bundle from three storages: 0 = flash, 1 = a copy in external RAM,
/// 2 = /sdcard/ASSETS.BIN. The RAM copy is made on first use and kept.
class AssetSources final
{
public:
    AssetSources(AssetSource& flash, RemovableFileSystem& sd, MemoryProvider& memory);

    AssetSource* select(std::uint8_t storage);
    AssetSource& flash() { return flash_; }
    void         fallback_to_flash();

private:
    class RamSource final : public AssetSource
    {
    public:
        void set_data(const std::uint8_t* data) { data_ = data; }
        bool read(const brick::interfaces::display::AssetDescriptor& asset, std::size_t offset,
                  std::uint8_t* destination, std::size_t bytes) override;

    private:
        const std::uint8_t* data_ = nullptr;
    };

    class SdSource final : public AssetSource
    {
    public:
        explicit SdSource(RemovableFileSystem& filesystem) : filesystem_(filesystem) {}
        bool begin();
        void close() { file_.close(); }
        bool read(const brick::interfaces::display::AssetDescriptor& asset, std::size_t offset,
                  std::uint8_t* destination, std::size_t bytes) override;

    private:
        RemovableFileSystem&                   filesystem_;
        brick::interfaces::storage::FileHandle file_;
    };

    bool prepare_ram_copy();
    void close_sd();

    AssetSource&                        flash_;
    RemovableFileSystem&                sd_;
    RamSource                           ram_source_;
    SdSource                            sd_source_;
    brick::core::image::AssetRepository repository_;
    brick::core::memory::MemoryBlock    ram_bundle_;
    bool                                ram_ready_ = false;
    bool                                sd_ready_  = false;
};
}  // namespace asset_stream_smoke
