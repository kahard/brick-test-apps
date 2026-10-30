#include "AssetBenchmark.h"

namespace asset_stream_smoke
{
AssetBenchmark::AssetBenchmark(TimeProvider& time, Logger& logger) : time_(time), logger_(logger)
{
    reset();
}

void AssetBenchmark::reset()
{
    frames_  = 0;
    started_ = time_.micros();
}

void AssetBenchmark::frame()
{
    if (++frames_ < 60)
        return;
    const std::uint64_t elapsed  = time_.micros() - started_;
    const std::uint64_t fps_x100 = elapsed > 0 ? 6000000000ULL / elapsed : 0;
    logger_.info("brick_jc1060_assets", "fps=%llu.%02llu", static_cast<unsigned long long>(fps_x100 / 100),
                 static_cast<unsigned long long>(fps_x100 % 100));
    reset();
}
}  // namespace asset_stream_smoke
