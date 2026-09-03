#pragma once
#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/time/ITimeProvider.h"
#include <cstdint>
class AssetBenchmark final
{
public:
    AssetBenchmark(brick::interfaces::time::ITimeProvider& time, brick::interfaces::logging::ILogger& logger);
    void reset();
    void frame();

private:
    brick::interfaces::time::ITimeProvider& time_;
    brick::interfaces::logging::ILogger&    logger_;
    std::uint32_t                           frames_  = 0;
    std::uint64_t                           started_ = 0;
};
