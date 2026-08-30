#pragma once

#include <cstddef>
#include <cstdint>

#include <pgmspace.h>

#include "brick/interfaces/display/AssetDescriptor.h"

class ProgmemAssetSource final : public brick::interfaces::display::IAssetSource
{
public:
    ProgmemAssetSource(const std::uint8_t* data, std::size_t size) : data_(data), size_(size) {}

    bool read(const brick::interfaces::display::AssetDescriptor& asset, std::size_t offset, std::uint8_t* destination,
              std::size_t bytes) override
    {
        if (data_ == nullptr || destination == nullptr || offset > asset.size || bytes > asset.size - offset
            || asset.offset > size_ || bytes > size_ - asset.offset - offset)
            return false;
        memcpy_P(destination, data_ + asset.offset + offset, bytes);
        return true;
    }

private:
    const std::uint8_t* data_ = nullptr;
    std::size_t         size_ = 0;
};
