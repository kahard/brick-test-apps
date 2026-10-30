#pragma once

#include "Types.h"
#include "brick/core/display/Screen.h"
#include "brick/core/memory/MemoryBlock.h"

namespace display_touch_smoke
{
/// One full-screen render buffer in external RAM.
class FrameBuffer final
{
public:
    FrameBuffer(DisplayDevice& display, MemoryProvider& memory);

    bool initialize();
    bool present();

    brick::interfaces::display::WritablePixelBuffer& pixels() { return pixels_; }

private:
    brick::core::display::Screen                    screen_;
    brick::core::memory::MemoryBlock                memory_;
    brick::interfaces::display::WritablePixelBuffer pixels_{};
};
}  // namespace display_touch_smoke
