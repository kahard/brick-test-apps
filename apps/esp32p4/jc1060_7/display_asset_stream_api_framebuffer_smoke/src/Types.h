#pragma once
#include "ApplicationConfig.h"
#include "brick/boards/esp32/p4/Jc1060Board.h"
#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/display/ITouchscreen.h"
#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/time/ITimeProvider.h"

using Board        = brick::boards::BoardTemplate<Features>;
using Display      = brick::interfaces::display::IDisplayDevice;
using Touchscreen  = brick::interfaces::display::ITouchscreen;
using Logger       = brick::interfaces::logging::ILogger;
using TimeProvider = brick::interfaces::time::ITimeProvider;
