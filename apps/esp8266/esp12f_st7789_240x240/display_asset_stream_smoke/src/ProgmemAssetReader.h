#pragma once

#include <pgmspace.h>

#include "brick/interfaces/display/IAssetReader.h"

namespace esp12f_st7789_asset_stream_smoke
{
class ProgmemAssetReader final : public brick::interfaces::display::IAssetReader
{
public:
    bool read(const brick::interfaces::display::ImageAsset& asset, std::size_t offset, std::uint8_t* destination,
              std::size_t bytes) override
    {
        if (asset.data == nullptr || destination == nullptr || offset > asset.data_size
            || bytes > asset.data_size - offset)
            return false;
        memcpy_P(destination, asset.data + offset, bytes);
        return true;
    }
};
}  // namespace esp12f_st7789_asset_stream_smoke
