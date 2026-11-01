#pragma once

#include <cstdint>

#include "Services.h"
#include "brick/core/time/Timer.h"

namespace display_framebuffer_smoke
{
/// Fills the driver's scan-out frame buffers alternately and presents them.
class FramebufferTest final
{
public:
    explicit FramebufferTest(const Services& services);
    Status initialize();
    void   update();

private:
    Status fill_and_present(std::uint8_t index, std::uint16_t color);

    DisplayDevice&           display_;
    FrameBufferDisplay&      frame_buffers_;
    TimeProvider&            time_;
    Logger&                  logger_;
    brick::core::time::Timer timer_;
    std::uint8_t             index_ = 0;
};
}  // namespace display_framebuffer_smoke
