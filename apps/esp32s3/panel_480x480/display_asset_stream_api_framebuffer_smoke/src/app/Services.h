#pragma once

#include "brick/interfaces/display/AssetDescriptor.h"
#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/display/IFrameBufferDisplay.h"
#include "brick/interfaces/display/ITouchscreen.h"
#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/memory/IMemoryProvider.h"
#include "brick/interfaces/storage/IRemovableFileSystem.h"
#include "brick/interfaces/time/ITimeProvider.h"

namespace asset_stream_smoke
{
using DisplayDevice       = brick::interfaces::display::IDisplayDevice;
using FrameBufferDisplay  = brick::interfaces::display::IFrameBufferDisplay;
using Touchscreen         = brick::interfaces::display::ITouchscreen;
using AssetSource         = brick::interfaces::display::IAssetSource;
using RemovableFileSystem = brick::interfaces::storage::IRemovableFileSystem;
using MemoryProvider      = brick::interfaces::memory::IMemoryProvider;
using TimeProvider        = brick::interfaces::time::ITimeProvider;
using Logger              = brick::interfaces::logging::ILogger;

/// What the application needs from a board, filled by the target entry point
/// (hardware board or PC simulator). display and frame_buffers are the same
/// panel driver seen through its two interfaces; flash_assets holds the
/// generated bundle (a flash partition on the target, a file on the PC); sd
/// may hold a copy as /sdcard/ASSETS.BIN.
struct Services
{
    DisplayDevice&       display;
    FrameBufferDisplay&  frame_buffers;
    Touchscreen&         touch;
    AssetSource&         flash_assets;
    RemovableFileSystem& sd;
    MemoryProvider&      memory;
    TimeProvider&        time;
    Logger&              logger;
};
}  // namespace asset_stream_smoke
