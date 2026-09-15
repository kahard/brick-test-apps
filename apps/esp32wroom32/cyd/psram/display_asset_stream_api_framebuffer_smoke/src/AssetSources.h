#pragma once

#include <memory>

#include "brick/interfaces/display/AssetDescriptor.h"
#include "brick/interfaces/storage/IFileSystem.h"

namespace cyd_asset_stream_smoke
{
class AssetSources final
{
public:
    explicit AssetSources(brick::interfaces::storage::IFileSystem& filesystem);
    ~AssetSources();

    bool                                      initialize();
    brick::interfaces::display::IAssetSource* select(std::uint8_t storage);
    brick::interfaces::display::IAssetSource& flash();
    void                                      fallback_to_flash();

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
}  // namespace cyd_asset_stream_smoke
