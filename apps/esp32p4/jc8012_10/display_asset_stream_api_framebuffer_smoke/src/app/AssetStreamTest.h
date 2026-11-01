#pragma once

#include "AssetBenchmark.h"
#include "AssetPlayback.h"
#include "Services.h"
#include "TouchInput.h"

namespace asset_stream_smoke
{
/// Plays the assets, switches mode on each touch and logs the frame rate.
class AssetStreamTest final
{
public:
    AssetStreamTest(AssetPlayback& playback, Touchscreen& touch, TimeProvider& time, Logger& logger);
    Status initialize();
    void   update();

private:
    void           log_mode();
    AssetPlayback& playback_;
    TouchInput     touch_;
    AssetBenchmark benchmark_;
    Logger&        logger_;
    bool           failed_ = false;
};
}  // namespace asset_stream_smoke
