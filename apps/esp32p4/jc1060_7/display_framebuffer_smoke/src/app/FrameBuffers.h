#pragma once

#include <cstddef>
#include <cstdint>

#include "Services.h"
#include "brick/core/display/Screen.h"
#include "brick/core/memory/MemoryBlock.h"

namespace display_framebuffer_smoke
{
/// Two application render buffers in external RAM, not exposed MIPI scan-out
/// buffers: the display driver owns the scan-out buffers and presentation.
/// Full-screen buffers never fall back to internal RAM.
class FrameBuffers final
{
public:
    FrameBuffers(DisplayDevice& display, MemoryProvider& memory);
    Status                                          initialize();
    brick::interfaces::display::WritablePixelBuffer buffer(std::uint8_t index);
    Status                                          present(std::uint8_t index);
    std::size_t                                     size_bytes() const { return pixels_[0].size(); }

private:
    brick::core::display::Screen     screen_;
    brick::core::memory::MemoryBlock pixels_[2];
};
}  // namespace display_framebuffer_smoke
