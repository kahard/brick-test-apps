#pragma once

#include <cstddef>
#include <cstdint>

#include "brick/Status.h"
#include "brick/interfaces/audio/IPcmOutput.h"
#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/display/ITouchscreen.h"
#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/time/ITimeProvider.h"

namespace audio_test
{

using Status = brick::Status;

/// What the audio test needs from a board, filled by the target entry point
/// (hardware board or PC simulator).
struct Services
{
    brick::interfaces::display::IDisplayDevice& display;
    brick::interfaces::display::ITouchscreen*   touch;  ///< nullptr on a board without touch
    brick::interfaces::audio::IPcmOutput&       audio;
    brick::interfaces::time::ITimeProvider&     time;
    brick::interfaces::logging::ILogger&        logger;
};

/// One text line: line_height (about 1.4 x the font size) and its pixels,
/// screen width x line_height.
struct Settings
{
    const char*    board         = "board";
    std::int32_t   line_height   = 28;
    std::uint16_t* line_pixels   = nullptr;
    std::size_t    line_capacity = 0;
};

}  // namespace audio_test
