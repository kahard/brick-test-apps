#pragma once

#include <memory>

#include "Types.h"
#include "brick/interfaces/display/AssetDescriptor.h"

namespace cyd_asset_stream_smoke
{
class AssetSources final
{
public:
    explicit AssetSources(Board& board);
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
