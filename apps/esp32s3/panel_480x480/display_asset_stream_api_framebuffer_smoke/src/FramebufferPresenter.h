#pragma once

#include <array>

#include "brick/core/image/AssetStreamer.h"
#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/display/IFrameBufferDisplay.h"

class FramebufferPresenter final
{
public:
    FramebufferPresenter(brick::interfaces::display::IDisplayDevice&      display,
                         brick::interfaces::display::IFrameBufferDisplay& framebuffers);
    bool initialize(const brick::interfaces::display::AssetDescriptor& asset,
                    brick::interfaces::display::IAssetSource&          source);
    bool present(const brick::interfaces::display::AssetDescriptor& asset,
                 brick::interfaces::display::IAssetSource&          source);

private:
    static constexpr std::size_t kScratchBytes = 480U * 20U * 2U;

    brick::interfaces::display::IDisplayDevice&      display_;
    brick::interfaces::display::IFrameBufferDisplay& framebuffers_;
    brick::core::image::AssetStreamer                streamer_;
    std::array<std::uint8_t, kScratchBytes>          scratch_{};
    brick::interfaces::display::WritablePixelBuffer  buffers_[2]{};
    std::uint8_t                                     active_ = 0;
};
