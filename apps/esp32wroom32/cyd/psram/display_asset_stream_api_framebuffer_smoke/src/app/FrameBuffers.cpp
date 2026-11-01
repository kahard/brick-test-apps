#include "FrameBuffers.h"

namespace cyd_asset_stream_smoke
{
FrameBuffers::FrameBuffers(MemoryProvider& memory)
    : buffers_{ brick::core::memory::MemoryBlock{ memory }, brick::core::memory::MemoryBlock{ memory } },
      scratch_(memory)
{
}

Status FrameBuffers::initialize()
{
    using brick::interfaces::memory::MemoryKind;
    for (brick::core::memory::MemoryBlock& buffer : buffers_)
        if (const Status status = buffer.allocate(kFrameBytes, MemoryKind::external); !is_ok(status))
            return status;
    return scratch_.allocate(kStripeBytes, MemoryKind::dma);
}

brick::interfaces::display::WritablePixelBuffer FrameBuffers::back_buffer() const
{
    const std::uint8_t back = static_cast<std::uint8_t>(front_ ^ 1U);
    return { buffers_[back].data(),
             kWidth,
             kHeight,
             static_cast<std::size_t>(kWidth) * 2U,
             brick::interfaces::display::PixelFormat::rgb565,
             false };
}

brick::interfaces::display::PixelBuffer FrameBuffers::stripe_buffer(std::uint16_t height) const
{
    return { scratch_.data(),
             kWidth,
             height,
             static_cast<std::size_t>(kWidth) * 2U,
             brick::interfaces::display::PixelFormat::rgb565,
             false };
}

std::uint8_t* FrameBuffers::scratch() const
{
    return scratch_.data();
}

std::size_t FrameBuffers::scratch_bytes() const
{
    return kStripeBytes;
}

void FrameBuffers::swap()
{
    front_ = static_cast<std::uint8_t>(front_ ^ 1U);
}
}  // namespace cyd_asset_stream_smoke
