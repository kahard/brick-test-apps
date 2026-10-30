#pragma once

#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/display/ITouchscreen.h"
#include "brick/interfaces/storage/IRemovableFileSystem.h"
#include "brick/interfaces/time/ITimeProvider.h"
#include "brick/interfaces/logging/ILogger.h"

/// What the application needs from a board, filled by the target entry point
/// (hardware board or PC simulator).
struct Services
{
    brick::interfaces::display::IDisplayDevice& display;
    brick::interfaces::display::ITouchscreen& touch;
    brick::interfaces::storage::IRemovableFileSystem& sd;
    brick::interfaces::time::ITimeProvider& time;
    brick::interfaces::logging::ILogger& logger;
};
