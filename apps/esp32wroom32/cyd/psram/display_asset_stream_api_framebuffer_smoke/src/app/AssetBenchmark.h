#pragma once

#include <cstdint>

#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/time/ITimeProvider.h"

namespace cyd_asset_stream_smoke
{
class AssetBenchmark final
{
public:
    AssetBenchmark(brick::interfaces::time::ITimeProvider& time, brick::interfaces::logging::ILogger& logger);

    void reset();
    void frame(const char* storage, std::uint64_t load_us, std::uint64_t present_us);

private:
    brick::interfaces::time::ITimeProvider& time_;
    brick::interfaces::logging::ILogger&    logger_;
    std::uint32_t                           frames_           = 0;
    std::uint64_t                           started_          = 0;
    std::uint64_t                           load_total_us_    = 0;
    std::uint64_t                           present_total_us_ = 0;
};
}  // namespace cyd_asset_stream_smoke
