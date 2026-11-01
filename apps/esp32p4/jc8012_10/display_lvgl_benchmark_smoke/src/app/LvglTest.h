#pragma once

#include "BenchmarkSeries.h"
#include "LvglRuntime.h"
#include "Services.h"

namespace lvgl_benchmark_smoke
{
class LvglTest final
{
public:
    explicit LvglTest(const Services& services);
    Status initialize();
    void   update();

private:
    DisplayDevice&  display_;
    Logger&         logger_;
    LvglRuntime     runtime_;
    LvglAssets      assets_;
    BenchmarkSeries series_;
};
}  // namespace lvgl_benchmark_smoke
