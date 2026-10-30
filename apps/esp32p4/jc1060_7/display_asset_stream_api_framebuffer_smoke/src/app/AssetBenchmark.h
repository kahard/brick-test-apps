#pragma once

#include <cstdint>

#include "Services.h"

namespace asset_stream_smoke
{
/// Logs the frame rate every 60 frames.
class AssetBenchmark final
{
public:
    AssetBenchmark(TimeProvider& time, Logger& logger);
    void reset();
    void frame();

private:
    TimeProvider& time_;
    Logger&       logger_;
    std::uint32_t frames_  = 0;
    std::uint64_t started_ = 0;
};
}  // namespace asset_stream_smoke
