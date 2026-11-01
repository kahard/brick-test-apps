#pragma once

#include "brick/Status.h"
#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/display/ITouchscreen.h"
#include "brick/interfaces/display/TouchscreenTypes.h"
#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/memory/IMemoryProvider.h"

namespace display_touch_smoke
{
using Status         = brick::Status;
using DisplayDevice  = brick::interfaces::display::IDisplayDevice;
using Touchscreen    = brick::interfaces::display::ITouchscreen;
using TouchPoint     = brick::interfaces::display::TouchPoint;
using MemoryProvider = brick::interfaces::memory::IMemoryProvider;
using Logger         = brick::interfaces::logging::ILogger;

/// What the application needs from a board, filled by the target entry point
/// (hardware board or PC simulator).
struct Services
{
    DisplayDevice&  display;
    Touchscreen&    touch;
    MemoryProvider& memory;
    Logger&         logger;
};
}  // namespace display_touch_smoke
