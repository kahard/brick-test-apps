#pragma once

#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/time/ITimeProvider.h"

namespace esp12f_st7789_spi_benchmark_smoke
{
using DisplayDevice = brick::interfaces::display::IDisplayDevice;
using TimeProvider  = brick::interfaces::time::ITimeProvider;
using Logger        = brick::interfaces::logging::ILogger;

/// What the application needs from a board, filled by the target entry point
/// (ESP-12F board or PC simulator).
struct Services
{
    DisplayDevice& display;
    TimeProvider&  time;
    Logger&        logger;
};
}  // namespace esp12f_st7789_spi_benchmark_smoke
