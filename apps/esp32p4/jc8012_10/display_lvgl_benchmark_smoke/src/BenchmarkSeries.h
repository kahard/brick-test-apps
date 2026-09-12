#pragma once
#include "LvglAssets.h"
#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/time/ITimeProvider.h"

// One frame per update, with a timed pause between series. No blocking 60-frame loop.
class BenchmarkSeries final
{
public:
    BenchmarkSeries(brick::interfaces::time::ITimeProvider& time, brick::interfaces::logging::ILogger& logger);
    void initialize(lv_display_t* display, LvglAssets& assets);
    void update();

private:
    brick::interfaces::time::ITimeProvider& time_;
    brick::interfaces::logging::ILogger&    logger_;
    lv_display_t*                           display_     = nullptr;
    LvglAssets*                             assets_      = nullptr;
    lv_obj_t*                               image_       = nullptr;
    bool                                    colors_      = false;
    std::uint32_t                           frames_      = 0;
    std::uint32_t                           next_series_ = 0;
    std::uint64_t                           started_     = 0;
};
