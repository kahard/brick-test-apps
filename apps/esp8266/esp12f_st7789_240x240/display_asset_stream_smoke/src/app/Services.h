#pragma once

#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/input/IButton.h"
#include "brick/interfaces/time/ITimeProvider.h"
#include "brick/interfaces/logging/ILogger.h"

namespace esp12f_st7789_asset_stream_smoke
{
using DisplayDevice = brick::interfaces::display::IDisplayDevice;
using Button = brick::interfaces::input::IButton;
using TimeProvider = brick::interfaces::time::ITimeProvider;
using Logger = brick::interfaces::logging::ILogger;

/// What the application needs from a board, filled by the target entry point
/// (ESP-12F board or PC simulator).
struct Services
{
    DisplayDevice& display;
    Button& button;
    TimeProvider& time;
    Logger& logger;
};
}  // namespace esp12f_st7789_asset_stream_smoke
