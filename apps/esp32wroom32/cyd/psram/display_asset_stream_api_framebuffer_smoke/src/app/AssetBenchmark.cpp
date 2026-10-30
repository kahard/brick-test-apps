#include "AssetBenchmark.h"

namespace cyd_asset_stream_smoke
{
namespace
{
    constexpr char          kTag[]           = "brick_cyd_asset";
    constexpr std::uint32_t kFramesPerReport = 30;

    /// Splits a value in hundredths into its integer and two-decimal parts.
    struct Hundredths
    {
        unsigned long long whole;
        unsigned long long fraction;
    };

    Hundredths hundredths(std::uint64_t value_x100)
    {
        return { static_cast<unsigned long long>(value_x100 / 100U), static_cast<unsigned long long>(value_x100 % 100U) };
    }
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

    // Integer arithmetic only; each value is reported with two decimals.
    const std::uint64_t elapsed_us = time_.micros() - started_;
    const Hundredths    fps     = hundredths(elapsed_us > 0 ? frames_ * 100000000ULL / elapsed_us : 0);
    const Hundredths    load    = hundredths(load_total_us_ / frames_ / 10U);
    const Hundredths    present = hundredths(present_total_us_ / frames_ / 10U);
    logger_.info(kTag, "storage=%s fps=%llu.%02llu load=%llu.%02llums present=%llu.%02llums", storage, fps.whole,
                 fps.fraction, load.whole, load.fraction, present.whole, present.fraction);
    reset();
}
}  // namespace cyd_asset_stream_smoke
