#pragma once

#include <cstddef>
#include <cstdint>

#include "Types.h"

namespace cyd_display_framebuffer_smoke
{

class FramebufferTest final
{
public:
    FramebufferTest(DisplayDevice& display, TimeProvider& time, Logger& logger);

    bool initialize();
    void update();

private:
    bool present(const std::uint8_t* framebuffer, std::uint64_t& transfer_us);
    void fill_frame(std::uint8_t* framebuffer, std::uint16_t color);
    void present_next_color();

    DisplayDevice& display_;
    TimeProvider&  time_;
    Logger&        logger_;
    std::uint8_t*  frames_[2]{};
    std::uint32_t  next_color_ms_     = 0;
    std::uint8_t   color_index_       = 0;
    std::uint32_t  measured_frames_   = 0;
    std::uint64_t  transfer_total_us_ = 0;
};

}  // namespace cyd_display_framebuffer_smoke
