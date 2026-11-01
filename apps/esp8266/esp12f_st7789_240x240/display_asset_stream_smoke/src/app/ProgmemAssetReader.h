#pragma once

#include "Services.h"
#include "brick/core/memory/ProgramMemory.h"

#include "brick/interfaces/display/IAssetReader.h"

namespace esp12f_st7789_asset_stream_smoke
{
class ProgmemAssetReader final : public brick::interfaces::display::IAssetReader
{
public:
    Status read(const brick::interfaces::display::ImageAsset& asset, std::size_t offset, std::uint8_t* destination,
                std::size_t bytes) override
    {
        if (asset.data == nullptr || destination == nullptr || offset > asset.data_size
            || bytes > asset.data_size - offset)
            return Status::error;
        brick_progmem_copy(destination, asset.data + offset, bytes);
        return Status::ok;
    }
};
}  // namespace esp12f_st7789_asset_stream_smoke
