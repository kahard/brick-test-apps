#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "Services.h"
#include "brick/core/image/AssetStreamer.h"

namespace asset_stream_smoke
{
/// Streams an asset into the driver's back scan-out buffer and flips at vsync.
class FramebufferPresenter final
{
public:
    FramebufferPresenter(DisplayDevice& display, FrameBufferDisplay& frame_buffers);
    bool initialize(const brick::interfaces::display::AssetDescriptor& asset, AssetSource& source);
    bool present(const brick::interfaces::display::AssetDescriptor& asset, AssetSource& source);

private:
    static constexpr std::size_t kScratchBytes = 480U * 20U * 2U;

    DisplayDevice&                                  display_;
    FrameBufferDisplay&                             frame_buffers_;
    brick::core::image::AssetStreamer               streamer_;
    std::array<std::uint8_t, kScratchBytes>         scratch_{};
    brick::interfaces::display::WritablePixelBuffer buffers_[2]{};
    std::uint8_t                                    active_ = 0;
};
}  // namespace asset_stream_smoke
