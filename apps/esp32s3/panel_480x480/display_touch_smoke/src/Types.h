#pragma once

#include "ApplicationConfig.h"
#include "brick/boards/esp32/s3/Panel480BoardTemplate.h"
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
using Board         = brick::platform::esp32::s3::Panel480BoardTemplate<DisplayTouchFeatures>;
}  // namespace display_touch_smoke
