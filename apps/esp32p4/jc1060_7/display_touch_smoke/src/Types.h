#pragma once

#include "ApplicationConfig.h"
#include "brick/boards/esp32/p4/Jc1060BoardTemplate.h"
#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/display/ITouchscreen.h"
#include "brick/interfaces/display/TouchscreenTypes.h"
#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/time/ITimeProvider.h"

namespace display_touch_smoke
{
using DisplayDevice = brick::interfaces::display::IDisplayDevice;
using Touchscreen   = brick::interfaces::display::ITouchscreen;
using TouchPoint    = brick::interfaces::display::TouchPoint;
using TimeProvider  = brick::interfaces::time::ITimeProvider;
using Logger        = brick::interfaces::logging::ILogger;
using Board         = brick::platform::esp32::p4::Jc1060BoardTemplate<Features>;
}  // namespace display_touch_smoke
