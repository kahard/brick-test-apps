#include "FramebufferPresenter.h"

namespace asset_stream_smoke
{
FramebufferPresenter::FramebufferPresenter(DisplayDevice& display, MemoryProvider& memory)
    : buffers_(display, memory), streamer_(display)
{
}

Status FramebufferPresenter::initialize(const brick::interfaces::display::AssetDescriptor& asset, AssetSource& source)
{
    if (const Status status = buffers_.initialize(); !is_ok(status))
        return status;
    return present(asset, source);
}

Status FramebufferPresenter::present(const brick::interfaces::display::AssetDescriptor& asset, AssetSource& source)
{
    if (!is_ok(streamer_.stream_to_buffer(asset, source, buffers_.buffer(next_), scratch_.data(), scratch_.size()))
        || !is_ok(buffers_.present(next_)))
        return Status::error;
    next_ ^= 1U;
    return Status::ok;
}
}  // namespace asset_stream_smoke
