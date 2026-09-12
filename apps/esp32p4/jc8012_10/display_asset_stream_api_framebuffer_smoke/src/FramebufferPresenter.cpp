#include "FramebufferPresenter.h"

FramebufferPresenter::FramebufferPresenter(brick::interfaces::display::IDisplayDevice& display)
    : buffers_(display), streamer_(display)
{
}

bool FramebufferPresenter::initialize(const brick::interfaces::display::AssetDescriptor& asset,
                                      brick::interfaces::display::IAssetSource&          source)
{
    return buffers_.initialize() && present(asset, source);
}

bool FramebufferPresenter::present(const brick::interfaces::display::AssetDescriptor& asset,
                                   brick::interfaces::display::IAssetSource&          source)
{
    if (!streamer_.stream_to_buffer(asset, source, buffers_.buffer(next_), scratch_.data(), scratch_.size())
        || !buffers_.present(next_))
        return false;
    next_ ^= 1U;
    return true;
}
