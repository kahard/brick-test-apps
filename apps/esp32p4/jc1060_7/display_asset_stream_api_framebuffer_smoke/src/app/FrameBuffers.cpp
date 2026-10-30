#include "FrameBuffers.h"

namespace asset_stream_smoke
{
FrameBuffers::FrameBuffers(DisplayDevice& display, MemoryProvider& memory)
    : screen_(display),
      pixels_{ brick::core::memory::MemoryBlock{ memory }, brick::core::memory::MemoryBlock{ memory } }
{
}

bool FrameBuffers::initialize()
{
    const brick::interfaces::display::DisplaySize size  = screen_.size();
    const std::size_t                             bytes = static_cast<std::size_t>(size.width) * size.height * 2;
    return pixels_[0].allocate(bytes, brick::interfaces::memory::MemoryKind::external)
           && pixels_[1].allocate(bytes, brick::interfaces::memory::MemoryKind::external);
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

bool FrameBuffers::present(std::uint8_t index)
{
    const brick::interfaces::display::WritablePixelBuffer writable = buffer(index);
    if (!writable.valid())
        return false;
    const brick::interfaces::display::PixelBuffer pixels{ writable.data,         writable.width,  writable.height,
                                                          writable.stride_bytes, writable.format, false };
    return screen_.present(pixels) && screen_.wait_for_transfer_complete(1000);
}
}  // namespace asset_stream_smoke
