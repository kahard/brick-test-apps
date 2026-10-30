#include "FramebufferPresenter.h"

namespace asset_stream_smoke
{
FramebufferPresenter::FramebufferPresenter(DisplayDevice& display, MemoryProvider& memory)
    : buffers_(display, memory), streamer_(display)
{
}

bool FramebufferPresenter::initialize(const brick::interfaces::display::AssetDescriptor& asset, AssetSource& source)
{
    return buffers_.initialize() && present(asset, source);
}

bool FramebufferPresenter::present(const brick::interfaces::display::AssetDescriptor& asset, AssetSource& source)
{
    if (!streamer_.stream_to_buffer(asset, source, buffers_.buffer(next_), scratch_.data(), scratch_.size())
        || !buffers_.present(next_))
        return false;
    next_ ^= 1U;
    return true;
}
}  // namespace asset_stream_smoke
