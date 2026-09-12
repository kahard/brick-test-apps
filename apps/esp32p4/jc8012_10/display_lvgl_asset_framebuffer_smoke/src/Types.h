#pragma once
#include "ApplicationConfig.h"
#include "brick/boards/esp32/p4/Jc8012BoardTemplate.h"
#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/display/ITouchscreen.h"
#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/time/ITimeProvider.h"

using Board        = brick::platform::esp32::p4::Jc8012BoardTemplate<Features>;
using Display      = brick::interfaces::display::IDisplayDevice;
using Touchscreen  = brick::interfaces::display::ITouchscreen;
using Logger       = brick::interfaces::logging::ILogger;
using TimeProvider = brick::interfaces::time::ITimeProvider;
