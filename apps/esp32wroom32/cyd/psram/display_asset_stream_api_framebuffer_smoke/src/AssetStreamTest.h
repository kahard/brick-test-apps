#pragma once

#include "AssetPlayback.h"
#include "AssetSources.h"
#include "FrameBuffers.h"
#include "FramebufferPresenter.h"
#include "TouchInput.h"
#include "Types.h"
#include "brick/core/image/AssetStreamer.h"

namespace cyd_asset_stream_smoke
{
class AssetStreamTest final
{
public:
    explicit AssetStreamTest(Board& board);
    bool initialize();
    void update();

private:
    Board&               board_;
    AssetSources         sources_{ board_ };
    FrameBuffers         buffers_;
    FramebufferPresenter presenter_{ board_.display(), buffers_ };
    AssetPlayback        playback_{ sources_, buffers_, presenter_, board_.display(), board_.time(), board_.logger() };
    TouchInput           touch_{ board_.touch() };
};
}  // namespace cyd_asset_stream_smoke
