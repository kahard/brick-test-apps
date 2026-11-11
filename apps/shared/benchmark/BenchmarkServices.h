#pragma once

#include <cstdint>

#include "brick/Status.h"
#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/display/IFrameBufferDisplay.h"
#include "brick/interfaces/display/ITouchscreen.h"
#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/memory/IMemoryProvider.h"
#include "brick/interfaces/time/ITimeProvider.h"

namespace benchmark_demo
{

using Status = brick::Status;

/// What the benchmark needs from a board, filled by the target entry point
/// (hardware board or PC simulator).
struct Services
{
    brick::interfaces::display::IDisplayDevice& display;
    /// The same panel's scan-out buffers (see DrawMode); nullptr on a
    /// display without them.
    brick::interfaces::display::IFrameBufferDisplay* frame_buffers;
    brick::interfaces::display::ITouchscreen*        touch;  ///< nullptr on a board without touch
    brick::interfaces::memory::IMemoryProvider&      memory;
    brick::interfaces::time::ITimeProvider&          time;
    brick::interfaces::logging::ILogger&             logger;
};

/// Where LVGL draws.
enum class DrawMode : std::uint8_t
{
    /// Straight into the two scan-out buffers (LVGL DIRECT): one page flip
    /// per frame, no tearing.
    frame_buffers,
    /// Into two buffers in internal RAM (fast to draw in); the display copies
    /// each area into the scan-out buffer off the screen and flips once per
    /// frame (LvglDisplayAdapter::create_buffered()): no tearing.
    ram_and_flip,
    /// Into two buffers in internal RAM, copied into the buffer on the screen
    /// (LVGL PARTIAL): no frame buffers needed, but moving objects tear.
    partial,
};

/// Board-specific choices, filled by the target entry point.
struct Settings
{
    const char*   board      = "board";  ///< shown on the summary and in the log
    std::uint32_t scene_ms   = 8000;     ///< how long each scene runs
    std::uint32_t summary_ms = 10000;    ///< how long the summary stays before the next round
    /// frame_buffers falls back to partial without frame buffers.
    DrawMode draw_mode = DrawMode::frame_buffers;
    /// ram_and_flip and partial: rows of each of the two LVGL draw buffers
    /// and the memory they come from.
    std::uint32_t                         partial_rows   = 40;
    brick::interfaces::memory::MemoryKind partial_memory = brick::interfaces::memory::MemoryKind::dma;
};

}  // namespace benchmark_demo
