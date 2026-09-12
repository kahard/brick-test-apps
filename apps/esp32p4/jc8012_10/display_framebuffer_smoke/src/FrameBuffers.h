#pragma once
#include "PixelMemory.h"
#include "brick/core/display/Screen.h"

// Two application render buffers, not exposed MIPI scan-out buffers.
// The MIPI driver owns the physical scan-out buffers and handles presentation.
class FrameBuffers final
{
public:
    explicit FrameBuffers(brick::interfaces::display::IDisplayDevice& display);
    bool                                            initialize();
    brick::interfaces::display::WritablePixelBuffer buffer(std::uint8_t index);
    bool                                            present(std::uint8_t index);
    std::size_t                                     size_bytes() const { return pixels_[0].size(); }

private:
    brick::core::display::Screen screen_;
    PixelMemory                  pixels_[2];
};
