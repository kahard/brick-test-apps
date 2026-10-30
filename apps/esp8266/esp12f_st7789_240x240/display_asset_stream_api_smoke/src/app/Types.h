#pragma once
#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/time/ITimeProvider.h"
namespace esp12f_st7789_asset_stream_api_smoke
{
using DisplayDevice = brick::interfaces::display::IDisplayDevice;
using Logger        = brick::interfaces::logging::ILogger;
using TimeProvider  = brick::interfaces::time::ITimeProvider;
}  // namespace esp12f_st7789_asset_stream_api_smoke
