#pragma once

#include <memory>

#include "brick/interfaces/display/AssetDescriptor.h"
namespace brick::platform::esp32
{
class SdmmcFileSystem;
}

class AssetSources final
{
public:
    explicit AssetSources(brick::platform::esp32::SdmmcFileSystem& sd);
    ~AssetSources();

    bool                                      initialize();
    brick::interfaces::display::IAssetSource* select(std::uint8_t storage);
    brick::interfaces::display::IAssetSource& flash();
    void                                      fallback_to_flash();

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
