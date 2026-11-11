#include "FrameBuffers.h"

#include <algorithm>

namespace lvgl_benchmark_smoke
{
FrameBuffers::FrameBuffers(DisplayDevice& display, MemoryProvider& memory)
    : screen_(display),
      pixels_{ brick::core::memory::MemoryBlock{ memory }, brick::core::memory::MemoryBlock{ memory } },
      alignment_(std::max<std::size_t>(64, display.capabilities().dma_alignment_bytes))
{
}

Status FrameBuffers::initialize()
{
    const brick::interfaces::display::DisplaySize size  = screen_.size();
    const std::size_t                             bytes = static_cast<std::size_t>(size.width) * size.height * 2;
    if (const Status status = pixels_[0].allocate(bytes, brick::interfaces::memory::MemoryKind::external, alignment_);
        !is_ok(status))
        return status;
    return pixels_[1].allocate(bytes, brick::interfaces::memory::MemoryKind::external, alignment_);
}

brick::interfaces::display::WritablePixelBuffer FrameBuffers::buffer(std::uint8_t index)
{
    if (index >= 2)
        return {};
    const brick::interfaces::display::DisplaySize size = screen_.size();
    return { pixels_[index].data(),
             size.width,
             size.height,
             static_cast<std::size_t>(size.width) * 2,
             brick::interfaces::display::PixelFormat::rgb565,
             false };
}
}  // namespace lvgl_benchmark_smoke
