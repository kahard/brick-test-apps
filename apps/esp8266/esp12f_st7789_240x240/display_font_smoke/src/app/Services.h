#pragma once

#include "brick/Status.h"
#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/input/IButton.h"
#include "brick/interfaces/logging/ILogger.h"

namespace esp12f_st7789_display_font_smoke
{
using Status        = brick::Status;
using DisplayDevice = brick::interfaces::display::IDisplayDevice;
using Button        = brick::interfaces::input::IButton;
using Logger        = brick::interfaces::logging::ILogger;

/// What the application needs from a board, filled by the target entry point
/// (ESP-12F board or PC simulator).
struct Services
{
    DisplayDevice& display;
    Button&        button;
    Logger&        logger;
};
}  // namespace esp12f_st7789_display_font_smoke
