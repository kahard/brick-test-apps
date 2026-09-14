#include "FrameBuffers.h"

#include "esp_heap_caps.h"

namespace cyd_asset_stream_smoke
{
FrameBuffers::~FrameBuffers()
{
    heap_caps_free(buffers_[0]);
    heap_caps_free(buffers_[1]);
    heap_caps_free(scratch_);
}

bool FrameBuffers::initialize()
{
    buffers_[0] = static_cast<std::uint8_t*>(heap_caps_malloc(kFrameBytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
    buffers_[1] = static_cast<std::uint8_t*>(heap_caps_malloc(kFrameBytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
    scratch_    = static_cast<std::uint8_t*>(heap_caps_malloc(kStripeBytes, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL));
    return buffers_[0] != nullptr && buffers_[1] != nullptr && scratch_ != nullptr;
}

brick::interfaces::display::WritablePixelBuffer FrameBuffers::back_buffer() const
{
    const std::uint8_t back = static_cast<std::uint8_t>(front_ ^ 1U);
    return { buffers_[back],
             kWidth,
             kHeight,
             static_cast<std::size_t>(kWidth) * 2U,
             brick::interfaces::display::PixelFormat::rgb565,
             false };
}

brick::interfaces::display::PixelBuffer FrameBuffers::stripe_buffer(std::uint16_t height) const
{
    return { scratch_,
             kWidth,
             height,
             static_cast<std::size_t>(kWidth) * 2U,
             brick::interfaces::display::PixelFormat::rgb565,
             false };
}

std::uint8_t* FrameBuffers::scratch() const
{
    return scratch_;
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
