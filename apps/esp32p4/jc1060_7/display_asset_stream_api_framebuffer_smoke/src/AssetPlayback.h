#pragma once

#include <cstdint>

#include "AssetModeController.h"

class AssetSources;
class FramebufferPresenter;

class AssetPlayback final
{
public:
    AssetPlayback(AssetSources& sources, FramebufferPresenter& presenter);

    bool initialize();
    bool advance_mode();
    bool present_next();

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
