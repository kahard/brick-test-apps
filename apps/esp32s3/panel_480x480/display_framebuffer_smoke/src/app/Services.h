#pragma once

#include "brick/Status.h"
#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/display/IFrameBufferDisplay.h"
#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/time/ITimeProvider.h"

namespace display_framebuffer_smoke
{
using Status             = brick::Status;
using DisplayDevice      = brick::interfaces::display::IDisplayDevice;
using FrameBufferDisplay = brick::interfaces::display::IFrameBufferDisplay;
using TimeProvider       = brick::interfaces::time::ITimeProvider;
using Logger             = brick::interfaces::logging::ILogger;

/// What the application needs from a board, filled by the target entry point
/// (hardware board or PC simulator). display and frame_buffers are the same
/// panel driver seen through its two interfaces.
struct Services
{
    DisplayDevice&      display;
    FrameBufferDisplay& frame_buffers;
    TimeProvider&       time;
    Logger&             logger;
};
}  // namespace display_framebuffer_smoke
