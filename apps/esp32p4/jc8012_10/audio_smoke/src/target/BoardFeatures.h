#pragma once

namespace audio_smoke
{

/// The board's display, touch screen and speaker.
struct BoardFeatures
{
    static constexpr bool display   = true;
    static constexpr bool touch     = true;
    static constexpr bool backlight = true;
    static constexpr bool sd        = false;
    static constexpr bool audio     = true;
    static constexpr bool logging   = true;
    static constexpr int  log_level = 0;
};

}  // namespace audio_smoke
