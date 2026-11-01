#pragma once

#include <cstddef>
#include <cstdint>

#include "Services.h"
#include "brick/core/memory/MemoryBlock.h"

namespace cyd_display_framebuffer_smoke
{

/// Two full frames in PSRAM; one is filled with the next color and sent to the
/// SPI panel, and the average full-frame transfer time is logged.
class FramebufferTest final
{
public:
    explicit FramebufferTest(const Services& services);

    Status initialize();
    void   update();

private:
    Status present(const std::uint8_t* framebuffer, std::uint64_t& transfer_us);
    void   fill_frame(std::uint8_t* framebuffer, std::uint16_t color);
    void   present_next_color();

    DisplayDevice&                   display_;
    TimeProvider&                    time_;
    Logger&                          logger_;
    brick::core::memory::MemoryBlock frames_[2];
    std::uint32_t                    next_color_ms_     = 0;
    std::uint8_t                     color_index_       = 0;
    std::uint32_t                    measured_frames_   = 0;
    std::uint64_t                    transfer_total_us_ = 0;
};

}  // namespace cyd_display_framebuffer_smoke
