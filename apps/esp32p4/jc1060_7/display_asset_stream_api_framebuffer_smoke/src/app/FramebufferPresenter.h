#pragma once

#include <array>
#include <cstdint>

#include "FrameBuffers.h"
#include "Services.h"
#include "brick/core/image/AssetStreamer.h"

namespace asset_stream_smoke
{
/// Streams an asset into the next render buffer and presents it.
class FramebufferPresenter final
{
public:
    FramebufferPresenter(DisplayDevice& display, MemoryProvider& memory);
    bool initialize(const brick::interfaces::display::AssetDescriptor& asset, AssetSource& source);
    bool present(const brick::interfaces::display::AssetDescriptor& asset, AssetSource& source);

private:
    FrameBuffers                        buffers_;
    brick::core::image::AssetStreamer   streamer_;
    std::array<std::uint8_t, 32 * 1024> scratch_{};
    std::uint8_t                        next_ = 0;
};
}  // namespace asset_stream_smoke
