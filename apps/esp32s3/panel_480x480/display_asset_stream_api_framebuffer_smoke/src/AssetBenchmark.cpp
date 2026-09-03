#include "AssetBenchmark.h"
AssetBenchmark::AssetBenchmark(brick::interfaces::time::ITimeProvider& time,
                               brick::interfaces::logging::ILogger&    logger)
    : time_(time), logger_(logger)
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
    const std::uint64_t elapsed = time_.micros() - started_;
    logger_.info("brick_st7701s_asset_api_fb", "fps=%.2f", 60000000.0 / static_cast<double>(elapsed));
    reset();
}
