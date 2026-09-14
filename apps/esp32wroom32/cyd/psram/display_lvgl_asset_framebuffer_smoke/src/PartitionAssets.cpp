#include "PartitionAssets.h"

namespace cyd_lvgl_asset_smoke
{
bool PartitionAssets::initialize()
{
    return source_.begin();
}

bool PartitionAssets::load(const brick::interfaces::display::AssetDescriptor& asset, std::uint8_t* destination)
{
    return destination != nullptr && source_.read(asset, 0, destination, asset.size);
}
}  // namespace cyd_lvgl_asset_smoke
