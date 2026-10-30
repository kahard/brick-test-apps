#include "FramebufferPresenter.h"

#include <cstring>

namespace cyd_asset_stream_smoke
{
namespace
{
    constexpr std::uint16_t kWidth        = 320;
    constexpr std::uint16_t kHeight       = 240;
    constexpr std::uint16_t kStripeHeight = 16;
}  // namespace

FramebufferPresenter::FramebufferPresenter(brick::interfaces::display::IDisplayDevice& display, FrameBuffers& buffers)
    : display_(display), buffers_(buffers)
{
}

bool FramebufferPresenter::present()
{
    const brick::interfaces::display::WritablePixelBuffer source = buffers_.back_buffer();
    for (std::uint16_t y = 0; y < kHeight; y += kStripeHeight)
    {
        const std::uint16_t height =
            kHeight - y < kStripeHeight ? static_cast<std::uint16_t>(kHeight - y) : kStripeHeight;
        const std::size_t bytes = static_cast<std::size_t>(height) * kWidth * 2U;
        std::memcpy(buffers_.scratch(), source.data + static_cast<std::size_t>(y) * source.stride_bytes, bytes);
        if (!display_.draw_buffer({ 0, y, kWidth, height }, buffers_.stripe_buffer(height)))
            return false;
    }
    if (!display_.wait_for_transfer_complete(1000))
        return false;
    buffers_.swap();
    return true;
}
}  // namespace cyd_asset_stream_smoke
