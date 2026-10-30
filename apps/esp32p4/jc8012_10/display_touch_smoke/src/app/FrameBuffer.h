#pragma once

#include "PixelMemory.h"
#include "brick/core/display/Screen.h"
#include "brick/interfaces/display/IDisplayDevice.h"

namespace display_touch_smoke
{
class FrameBuffer final
{
public:
    explicit FrameBuffer(brick::interfaces::display::IDisplayDevice& display);

    bool initialize();
    bool present();

    brick::interfaces::display::WritablePixelBuffer& pixels() { return pixels_; }

private:
    brick::core::display::Screen                    screen_;
    PixelMemory                                     memory_;
    brick::interfaces::display::WritablePixelBuffer pixels_{};
};
}  // namespace display_touch_smoke
