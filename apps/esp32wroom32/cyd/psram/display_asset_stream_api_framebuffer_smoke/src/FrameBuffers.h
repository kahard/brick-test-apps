#pragma once

#include <cstddef>
#include <cstdint>

#include "brick/interfaces/display/PixelBuffer.h"
#include "brick/interfaces/display/WritablePixelBuffer.h"

namespace cyd_asset_stream_smoke
{
class FrameBuffers final
{
public:
    ~FrameBuffers();
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

    std::uint8_t* buffers_[2] = {};
    std::uint8_t* scratch_    = nullptr;
    std::uint8_t  front_      = 0;
};
}  // namespace cyd_asset_stream_smoke
