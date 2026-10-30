#pragma once

#include "AssetPlayback.h"
#include "AssetSources.h"
#include "FrameBuffers.h"
#include "FramebufferPresenter.h"
#include "Services.h"
#include "TouchInput.h"

namespace cyd_asset_stream_smoke
{
/// Plays the assets through PSRAM frames, switches mode on each touch and
/// logs load and presentation times.
class AssetStreamTest final
{
public:
    explicit AssetStreamTest(const Services& services);
    bool initialize();
    void update();

private:
    Logger&              logger_;
    AssetSources         sources_;
    FrameBuffers         buffers_;
    FramebufferPresenter presenter_;
    AssetPlayback        playback_;
    TouchInput           touch_;
};
}  // namespace cyd_asset_stream_smoke
