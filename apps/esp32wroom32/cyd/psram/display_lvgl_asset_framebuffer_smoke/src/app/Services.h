#pragma once

#include "brick/Status.h"
#include "brick/interfaces/display/AssetDescriptor.h"
#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/display/ITouchscreen.h"
#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/memory/IMemoryProvider.h"
#include "brick/interfaces/time/ITimeProvider.h"

namespace cyd_lvgl_asset_smoke
{
using Status         = brick::Status;
using DisplayDevice  = brick::interfaces::display::IDisplayDevice;
using Touchscreen    = brick::interfaces::display::ITouchscreen;
using AssetSource    = brick::interfaces::display::IAssetSource;
using MemoryProvider = brick::interfaces::memory::IMemoryProvider;
using TimeProvider   = brick::interfaces::time::ITimeProvider;
using Logger         = brick::interfaces::logging::ILogger;

/// What the application needs from a board, filled by the target entry point
/// (hardware board or PC simulator). assets holds the generated bundle: a
/// flash partition on the target, a file on the PC.
struct Services
{
    DisplayDevice&  display;
    Touchscreen&    touch;
    AssetSource&    assets;
    MemoryProvider& memory;
    TimeProvider&   time;
    Logger&         logger;
};
}  // namespace cyd_lvgl_asset_smoke
