#pragma once

#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/input/IButton.h"

namespace esp12f_st7789_display_font_smoke
{
using DisplayDevice = brick::interfaces::display::IDisplayDevice;
using Button = brick::interfaces::input::IButton;

/// What the application needs from a board, filled by the target entry point
/// (ESP-12F board or PC simulator).
struct Services
{
    DisplayDevice& display;
    Button& button;
};
}  // namespace esp12f_st7789_display_font_smoke
