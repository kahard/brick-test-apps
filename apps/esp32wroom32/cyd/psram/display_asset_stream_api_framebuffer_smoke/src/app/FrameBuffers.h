#pragma once

#include <cstddef>
#include <cstdint>

#include "Services.h"
#include "brick/core/memory/MemoryBlock.h"
#include "brick/interfaces/display/PixelBuffer.h"
#include "brick/interfaces/display/WritablePixelBuffer.h"

namespace cyd_asset_stream_smoke
{
/// Two full frames in PSRAM and one DMA-capable stripe in internal RAM that
/// the SPI panel is fed from.
class FrameBuffers final
{
public:
    explicit FrameBuffers(MemoryProvider& memory);
    bool                                            initialize();
    brick::interfaces::display::WritablePixelBuffer back_buffer() const;
    brick::interfaces::display::PixelBuffer         stripe_buffer(std::uint16_t height) const;
    std::uint8_t*                                   scratch() const;
    std::size_t                                     scratch_bytes() const;
    void                                            swap();

private:
    static constexpr std::uint16_t kWidth        = 320;
    static constexpr std::uint16_t kHeight       = 240;
    static constexpr std::uint16_t kStripeHeight = 16;
    static constexpr std::size_t   kFrameBytes   = static_cast<std::size_t>(kWidth) * kHeight * 2U;
    static constexpr std::size_t   kStripeBytes  = static_cast<std::size_t>(kWidth) * kStripeHeight * 2U;

    brick::core::memory::MemoryBlock buffers_[2];
    brick::core::memory::MemoryBlock scratch_;
    std::uint8_t                     front_ = 0;
};
}  // namespace cyd_asset_stream_smoke
