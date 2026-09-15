#pragma once
#include "ApplicationConfig.h"
#include "brick/boards/esp8266/Esp12fSt7789BoardTemplate.h"
#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/input/IButton.h"
#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/time/ITimeProvider.h"
namespace esp12f_st7789_asset_stream_smoke
{
using Board         = brick::platform::esp8266::Esp12fSt7789BoardTemplate<Features>;
using DisplayDevice = brick::interfaces::display::IDisplayDevice;
using Button        = brick::interfaces::input::IButton;
using Logger        = brick::interfaces::logging::ILogger;
using TimeProvider  = brick::interfaces::time::ITimeProvider;
}  // namespace esp12f_st7789_asset_stream_smoke
