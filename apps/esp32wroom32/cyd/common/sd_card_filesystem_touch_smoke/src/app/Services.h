#pragma once

#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/display/ITouchscreen.h"
#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/storage/IRemovableFileSystem.h"
#include "brick/interfaces/time/ITimeProvider.h"

namespace cyd_sd_card_filesystem_touch_smoke
{
using DisplayDevice = brick::interfaces::display::IDisplayDevice;
using SdCard        = brick::interfaces::storage::IRemovableFileSystem;
using Touchscreen   = brick::interfaces::display::ITouchscreen;
using TimeProvider  = brick::interfaces::time::ITimeProvider;
using Logger        = brick::interfaces::logging::ILogger;

/// What the application needs from a board, filled by the target entry point
/// (hardware board or PC simulator).
struct Services
{
    DisplayDevice& display;
    SdCard&        sd;
    Touchscreen&   touch;
    TimeProvider&  time;
    Logger&        logger;
};
}  // namespace cyd_sd_card_filesystem_touch_smoke
