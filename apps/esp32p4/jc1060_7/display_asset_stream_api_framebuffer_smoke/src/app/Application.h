#pragma once

#include "AssetPlayback.h"
#include "AssetSources.h"
#include "AssetStreamTest.h"
#include "FramebufferPresenter.h"
#include "Services.h"

namespace asset_stream_smoke
{
/// Portable application; the board is started by the target entry point.
class Application final
{
public:
    explicit Application(const Services& services);

    Status initialize();
    void   update();

private:
    AssetSources         sources_;
    FramebufferPresenter presenter_;
    AssetPlayback        playback_;
    AssetStreamTest      test_;
};
}  // namespace asset_stream_smoke
