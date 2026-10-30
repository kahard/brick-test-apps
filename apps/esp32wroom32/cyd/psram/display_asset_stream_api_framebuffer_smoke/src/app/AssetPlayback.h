#pragma once

#include <cstdint>

#include "AssetBenchmark.h"
#include "AssetModeController.h"
#include "AssetSources.h"
#include "FrameBuffers.h"
#include "FramebufferPresenter.h"
#include "brick/core/image/AssetStreamer.h"

namespace cyd_asset_stream_smoke
{
class AssetPlayback final
{
public:
    AssetPlayback(AssetSources& sources, FrameBuffers& buffers, FramebufferPresenter& presenter,
                  brick::interfaces::display::IDisplayDevice& display, brick::interfaces::time::ITimeProvider& time,
                  brick::interfaces::logging::ILogger& logger);

    bool initialize();
    bool advance_mode();
    bool present_next();

    const char* storage_name() const;
    bool        backgrounds() const;

private:
    void fallback_to_flash();

    AssetSources&                           sources_;
    FrameBuffers&                           buffers_;
    FramebufferPresenter&                   presenter_;
    brick::core::image::AssetStreamer       streamer_;
    brick::interfaces::time::ITimeProvider& time_;
    AssetBenchmark                          benchmark_;
    AssetModeController                     mode_;
    std::uint32_t                           frame_ = 0;
};
}  // namespace cyd_asset_stream_smoke
