#pragma once

#include "ApplicationConfig.h"
#include "brick/boards/esp32/wroom32/CydBoard.h"
#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/time/ITimeProvider.h"

namespace cyd_display_framebuffer_smoke
{

using Board         = brick::platform::esp32::CydBoardTemplate<FramebufferFeatures>;
using DisplayDevice = brick::interfaces::display::IDisplayDevice;
using Logger        = brick::interfaces::logging::ILogger;
using TimeProvider  = brick::interfaces::time::ITimeProvider;

}  // namespace cyd_display_framebuffer_smoke
