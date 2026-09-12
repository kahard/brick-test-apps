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
    logger_.info("brick_jc8012_assets", "fps=%.2f", elapsed ? 60000000.0 / static_cast<double>(elapsed) : 0.0);
    reset();
}
