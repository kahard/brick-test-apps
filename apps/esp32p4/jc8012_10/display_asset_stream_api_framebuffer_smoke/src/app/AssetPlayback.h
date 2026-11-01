#pragma once

#include <cstdint>

#include "AssetModeController.h"
#include "Services.h"

namespace asset_stream_smoke
{
class AssetSources;
class FramebufferPresenter;

/// Alternates two assets of the current mode from the current storage.
class AssetPlayback final
{
public:
    AssetPlayback(AssetSources& sources, FramebufferPresenter& presenter);

    Status initialize();
    Status advance_mode();
    Status present_next();

    std::uint8_t storage() const;
    bool         backgrounds() const;

private:
    void fallback_to_flash();

    AssetSources&         sources_;
    FramebufferPresenter& presenter_;
    AssetModeController   mode_;
    std::uint32_t         frame_   = 0;
    std::uint8_t          storage_ = 0;
};
}  // namespace asset_stream_smoke
