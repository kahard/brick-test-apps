#pragma once

#include "brick/Status.h"
#include "brick/interfaces/communication/ISerialPort.h"
#include "brick/interfaces/display/IBacklight.h"
#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/display/ITouchscreen.h"
#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/memory/IMemoryProvider.h"
#include "brick/interfaces/time/ITimeProvider.h"

namespace cyd_color_calibration
{
using Status = brick::Status;

using DisplayDevice  = brick::interfaces::display::IDisplayDevice;
using Backlight      = brick::interfaces::display::IBacklight;
using Touchscreen    = brick::interfaces::display::ITouchscreen;
using SerialPort     = brick::interfaces::communication::ISerialPort;
using MemoryProvider = brick::interfaces::memory::IMemoryProvider;
using TimeProvider   = brick::interfaces::time::ITimeProvider;
using Logger         = brick::interfaces::logging::ILogger;

/// What the application needs from a board, filled by the target entry point
/// (hardware board or PC simulator). serial carries the commands of
/// tools/color_calibration_gui.py.
struct Services
{
    DisplayDevice&  display;
    Backlight&      backlight;
    Touchscreen&    touch;
    SerialPort&     serial;
    MemoryProvider& memory;
    TimeProvider&   time;
    Logger&         logger;
};

}  // namespace cyd_color_calibration
