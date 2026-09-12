#pragma once
#include "AssetBenchmark.h"
#include "AssetPlayback.h"
#include "TouchInput.h"

class AssetStreamTest final
{
public:
    AssetStreamTest(AssetPlayback& playback, brick::interfaces::display::ITouchscreen& touch,
                    brick::interfaces::time::ITimeProvider& time, brick::interfaces::logging::ILogger& logger);
    bool initialize();
    void update();

private:
    void                                 log_mode();
    AssetPlayback&                       playback_;
    TouchInput                           touch_;
    AssetBenchmark                       benchmark_;
    brick::interfaces::logging::ILogger& logger_;
    bool                                 failed_ = false;
};
