#pragma once

#include "brick/Status.h"
#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/memory/IMemoryProvider.h"
#include "brick/interfaces/time/ITimeProvider.h"

namespace cyd_display_framebuffer_smoke
{
using Status         = brick::Status;
using DisplayDevice  = brick::interfaces::display::IDisplayDevice;
using MemoryProvider = brick::interfaces::memory::IMemoryProvider;
using TimeProvider   = brick::interfaces::time::ITimeProvider;
using Logger         = brick::interfaces::logging::ILogger;

/// What the application needs from a board, filled by the target entry point
/// (hardware board or PC simulator).
struct Services
{
    DisplayDevice&  display;
    MemoryProvider& memory;
    TimeProvider&   time;
    Logger&         logger;
};
}  // namespace cyd_display_framebuffer_smoke
