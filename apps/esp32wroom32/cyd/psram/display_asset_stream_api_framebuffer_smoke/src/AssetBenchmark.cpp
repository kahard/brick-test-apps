#include "AssetBenchmark.h"

namespace cyd_asset_stream_smoke
{
namespace
{
    constexpr char          kTag[]           = "brick_cyd_asset";
    constexpr std::uint32_t kFramesPerReport = 30;
}  // namespace

AssetBenchmark::AssetBenchmark(brick::interfaces::time::ITimeProvider& time,
                               brick::interfaces::logging::ILogger&    logger)
    : time_(time), logger_(logger)
{
    reset();
}

void AssetBenchmark::reset()
{
    frames_           = 0;
    load_total_us_    = 0;
    present_total_us_ = 0;
    started_          = time_.micros();
}

void AssetBenchmark::frame(const char* storage, std::uint64_t load_us, std::uint64_t present_us)
{
    ++frames_;
    load_total_us_ += load_us;
    present_total_us_ += present_us;
    if (frames_ < kFramesPerReport)
        return;

    const std::uint64_t elapsed_us = time_.micros() - started_;
    const double        fps        = static_cast<double>(frames_) * 1000000.0 / static_cast<double>(elapsed_us);
    logger_.info(kTag, "storage=%s fps=%.2f load=%.2fms present=%.2fms", storage, fps,
                 static_cast<double>(load_total_us_) / static_cast<double>(frames_) / 1000.0,
                 static_cast<double>(present_total_us_) / static_cast<double>(frames_) / 1000.0);
    reset();
}
}  // namespace cyd_asset_stream_smoke
