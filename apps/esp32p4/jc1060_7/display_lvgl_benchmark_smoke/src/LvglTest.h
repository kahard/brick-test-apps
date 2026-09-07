#pragma once
#include "BenchmarkSeries.h"
#include "LvglRuntime.h"

class LvglTest final
{
public:
    LvglTest(brick::interfaces::display::IDisplayDevice& display, brick::interfaces::time::ITimeProvider& time,
             brick::interfaces::logging::ILogger& logger);
    bool initialize();
    void update();

private:
    brick::interfaces::display::IDisplayDevice& display_;
    brick::interfaces::logging::ILogger&        logger_;
    LvglRuntime                                 runtime_;
    LvglAssets                                  assets_;
    BenchmarkSeries                             series_;
};
