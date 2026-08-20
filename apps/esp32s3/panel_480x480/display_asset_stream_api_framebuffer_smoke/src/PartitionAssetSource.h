#pragma once

#include <cstddef>
#include <cstdint>

#include "brick/interfaces/display/AssetDescriptor.h"
#include "esp_partition.h"

class PartitionAssetSource final : public brick::interfaces::display::IAssetSource
{
public:
    explicit PartitionAssetSource(const char* label) : label_(label) {}

    bool begin()
    {
        partition_ = esp_partition_find_first(ESP_PARTITION_TYPE_DATA,
                                               ESP_PARTITION_SUBTYPE_ANY, label_);
        return partition_ != nullptr;
    }

    bool read(const brick::interfaces::display::AssetDescriptor& asset,
              std::size_t offset,
              std::uint8_t* destination,
              std::size_t bytes) override
    {
        if (partition_ == nullptr || destination == nullptr || offset > asset.size ||
            bytes > asset.size - offset || asset.offset > partition_->size ||
            bytes > partition_->size - asset.offset - offset)
            return false;
        return esp_partition_read(partition_, asset.offset + offset, destination, bytes) == ESP_OK;
    }

private:
    const char* label_ = nullptr;
    const esp_partition_t* partition_ = nullptr;
};
