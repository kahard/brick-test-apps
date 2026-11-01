#include "FramebufferPresenter.h"

namespace asset_stream_smoke
{
FramebufferPresenter::FramebufferPresenter(DisplayDevice& display, FrameBufferDisplay& frame_buffers)
    : display_(display), frame_buffers_(frame_buffers), streamer_(display)
{
}

Status FramebufferPresenter::initialize(const brick::interfaces::display::AssetDescriptor& asset, AssetSource& source)
{
    if (frame_buffers_.frame_buffer_count() != 2)
        return Status::not_supported;
    for (std::uint8_t index = 0; index < 2; ++index)
    {
        if (const Status status = frame_buffers_.get_frame_buffer(index, buffers_[index]); !is_ok(status))
            return status;
        if (const Status status =
                streamer_.stream_to_buffer(asset, source, buffers_[index], scratch_.data(), scratch_.size());
            !is_ok(status))
            return status;
    }
    return frame_buffers_.present_frame_buffer(0);
}

Status FramebufferPresenter::present(const brick::interfaces::display::AssetDescriptor& asset, AssetSource& source)
{
    const std::uint8_t back = active_ ^ 1U;
    if (!is_ok(streamer_.stream_to_buffer(asset, source, buffers_[back], scratch_.data(), scratch_.size()))
        || !is_ok(display_.wait_for_vsync(100)) || !is_ok(frame_buffers_.present_frame_buffer(back)))
        return Status::error;
    active_ = back;
    return Status::ok;
}
}  // namespace asset_stream_smoke
