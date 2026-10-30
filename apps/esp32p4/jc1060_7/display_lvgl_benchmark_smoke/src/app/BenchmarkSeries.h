#pragma once

#include <cstdint>

#include "LvglAssets.h"
#include "Services.h"

namespace lvgl_benchmark_smoke
{
/// One frame per update, with a timed pause between series. No blocking 60-frame loop.
class BenchmarkSeries final
{
public:
    BenchmarkSeries(TimeProvider& time, Logger& logger);
    void initialize(lv_display_t* display, LvglAssets& assets);
    void update();

private:
    TimeProvider& time_;
    Logger&       logger_;
    lv_display_t* display_     = nullptr;
    LvglAssets*   assets_      = nullptr;
    lv_obj_t*     image_       = nullptr;
    bool          colors_      = false;
    std::uint32_t frames_      = 0;
    std::uint32_t next_series_ = 0;
    std::uint64_t started_     = 0;
};
}  // namespace lvgl_benchmark_smoke
