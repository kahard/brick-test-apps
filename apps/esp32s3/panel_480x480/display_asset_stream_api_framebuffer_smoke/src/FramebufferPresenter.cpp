#include "FramebufferPresenter.h"

FramebufferPresenter::FramebufferPresenter(brick::interfaces::display::IDisplayDevice&      display,
                                           brick::interfaces::display::IFrameBufferDisplay& framebuffers)
    : display_(display), framebuffers_(framebuffers), streamer_(display)
{
}

bool FramebufferPresenter::initialize(const brick::interfaces::display::AssetDescriptor& asset,
                                      brick::interfaces::display::IAssetSource&          source)
{
    if (framebuffers_.frame_buffer_count() != 2 || !framebuffers_.get_frame_buffer(0, buffers_[0])
        || !framebuffers_.get_frame_buffer(1, buffers_[1]))
        return false;
    return streamer_.stream_to_buffer(asset, source, buffers_[0], scratch_.data(), scratch_.size())
           && streamer_.stream_to_buffer(asset, source, buffers_[1], scratch_.data(), scratch_.size())
           && framebuffers_.present_frame_buffer(0);
}

bool FramebufferPresenter::present(const brick::interfaces::display::AssetDescriptor& asset,
                                   brick::interfaces::display::IAssetSource&          source)
{
    const std::uint8_t back = active_ ^ 1U;
    if (!streamer_.stream_to_buffer(asset, source, buffers_[back], scratch_.data(), scratch_.size())
        || !display_.wait_for_vsync(100) || !framebuffers_.present_frame_buffer(back))
        return false;
    active_ = back;
    return true;
}
