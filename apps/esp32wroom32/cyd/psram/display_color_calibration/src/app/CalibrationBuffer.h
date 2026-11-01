#pragma once

#include <cstddef>
#include <cstdint>

#include "Services.h"
#include "brick/core/memory/MemoryBlock.h"

namespace cyd_color_calibration
{

/// One DMA-capable stripe of the screen, sent to the display at a time.
class CalibrationBuffer final
{
public:
    explicit CalibrationBuffer(MemoryProvider& memory) : block_(memory) {}

    Status         initialize();
    std::uint16_t* pixels() const;

    static constexpr std::uint16_t width() { return kWidth; }
    static constexpr std::uint16_t height() { return kHeight; }

private:
    static constexpr std::uint16_t   kWidth      = 320U;
    static constexpr std::uint16_t   kHeight     = 16U;
    static constexpr std::size_t     kPixelCount = kWidth * kHeight;
    brick::core::memory::MemoryBlock block_;
};

}  // namespace cyd_color_calibration
