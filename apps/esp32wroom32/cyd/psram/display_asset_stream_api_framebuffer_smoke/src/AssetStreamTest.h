#pragma once

#include "AssetPlayback.h"
#include "AssetSources.h"
#include "FrameBuffers.h"
#include "FramebufferPresenter.h"
#include "TouchInput.h"
#include "brick/core/image/AssetStreamer.h"
#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/display/ITouchscreen.h"
#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/storage/IFileSystem.h"
#include "brick/interfaces/time/ITimeProvider.h"

namespace cyd_asset_stream_smoke
{
class AssetStreamTest final
{
public:
    AssetStreamTest(brick::interfaces::display::IDisplayDevice& display,
                    brick::interfaces::storage::IFileSystem& filesystem,
                    brick::interfaces::display::ITouchscreen& touch, brick::interfaces::time::ITimeProvider& time,
                    brick::interfaces::logging::ILogger& logger);
    bool initialize();
    void update();

private:
    brick::interfaces::time::ITimeProvider& time_;
    brick::interfaces::logging::ILogger& logger_;
    AssetSources         sources_;
    FrameBuffers         buffers_;
    FramebufferPresenter presenter_;
    AssetPlayback        playback_;
    TouchInput           touch_;
};
}  // namespace cyd_asset_stream_smoke
