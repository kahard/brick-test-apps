#pragma once
#include "FrameBuffers.h"
#include "brick/core/image/AssetStreamer.h"
#include <array>

class FramebufferPresenter final
{
public:
    explicit FramebufferPresenter(brick::interfaces::display::IDisplayDevice& display);
    bool initialize(const brick::interfaces::display::AssetDescriptor& asset,
                    brick::interfaces::display::IAssetSource&          source);
    bool present(const brick::interfaces::display::AssetDescriptor& asset,
                 brick::interfaces::display::IAssetSource&          source);

private:
    FrameBuffers                        buffers_;
    brick::core::image::AssetStreamer   streamer_;
    std::array<std::uint8_t, 32 * 1024> scratch_{};
    std::uint8_t                        next_ = 0;
};
