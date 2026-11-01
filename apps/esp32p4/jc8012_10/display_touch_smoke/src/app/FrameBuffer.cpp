#include "FrameBuffer.h"

#include <cstddef>
#include <cstdint>

namespace display_touch_smoke
{
FrameBuffer::FrameBuffer(DisplayDevice& display, MemoryProvider& memory) : screen_(display), memory_(memory)
{
}

Status FrameBuffer::initialize()
{
    const brick::interfaces::display::DisplaySize size = screen_.size();
    const std::size_t                             bytes =
        static_cast<std::size_t>(size.width) * static_cast<std::size_t>(size.height) * sizeof(std::uint16_t);
    if (const Status status = memory_.allocate(bytes, brick::interfaces::memory::MemoryKind::external); !is_ok(status))
        return status;

    pixels_ = { memory_.data(),
                size.width,
                size.height,
                static_cast<std::size_t>(size.width) * sizeof(std::uint16_t),
                brick::interfaces::display::PixelFormat::rgb565,
                false };
    return pixels_.valid() ? Status::ok : Status::error;
}

Status FrameBuffer::present()
{
    const brick::interfaces::display::PixelBuffer read_only{ pixels_.data,         pixels_.width,  pixels_.height,
                                                             pixels_.stride_bytes, pixels_.format, false };
    if (const Status status = screen_.present(read_only); !is_ok(status))
        return status;
    return screen_.wait_for_transfer_complete(2000);
}
}  // namespace display_touch_smoke
