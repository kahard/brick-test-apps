#include "FramebufferPresenter.h"

namespace asset_stream_smoke
{
FramebufferPresenter::FramebufferPresenter(DisplayDevice& display, FrameBufferDisplay& frame_buffers)
    : display_(display), frame_buffers_(frame_buffers), streamer_(display)
{
}

bool FramebufferPresenter::initialize(const brick::interfaces::display::AssetDescriptor& asset, AssetSource& source)
{
    if (frame_buffers_.frame_buffer_count() != 2 || !frame_buffers_.get_frame_buffer(0, buffers_[0])
        || !frame_buffers_.get_frame_buffer(1, buffers_[1]))
        return false;
    return streamer_.stream_to_buffer(asset, source, buffers_[0], scratch_.data(), scratch_.size())
           && streamer_.stream_to_buffer(asset, source, buffers_[1], scratch_.data(), scratch_.size())
           && frame_buffers_.present_frame_buffer(0);
}

bool FramebufferPresenter::present(const brick::interfaces::display::AssetDescriptor& asset, AssetSource& source)
{
    const std::uint8_t back = active_ ^ 1U;
    if (!streamer_.stream_to_buffer(asset, source, buffers_[back], scratch_.data(), scratch_.size())
        || !display_.wait_for_vsync(100) || !frame_buffers_.present_frame_buffer(back))
        return false;
    active_ = back;
    return true;
}
}  // namespace asset_stream_smoke
