#pragma once

#include <cstdint>

#include "brick/interfaces/display/AssetDescriptor.h"
#include "brick/platform/esp32/PartitionAssetSource.h"

namespace cyd_lvgl_asset_smoke
{
class PartitionAssets final
{
public:
    bool initialize();
    bool load(const brick::interfaces::display::AssetDescriptor& asset, std::uint8_t* destination);

private:
    brick::platform::esp32::PartitionAssetSource source_{ "assets" };
};
}  // namespace cyd_lvgl_asset_smoke
