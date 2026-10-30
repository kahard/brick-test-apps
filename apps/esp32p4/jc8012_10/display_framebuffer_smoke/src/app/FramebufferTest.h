#pragma once

#include <cstdint>

#include "FrameBuffers.h"
#include "Services.h"
#include "brick/core/time/Timer.h"

namespace display_framebuffer_smoke
{
class FramebufferTest final
{
public:
    explicit FramebufferTest(const Services& services);
    bool initialize();
    void update();

private:
    bool                         draw_next();
    FrameBuffers                 buffers_;
    brick::core::display::Screen screen_;
    TimeProvider&                time_;
    Logger&                      logger_;
    brick::core::time::Timer     timer_;
    std::uint8_t                 index_  = 0;
    bool                         failed_ = false;
};
}  // namespace display_framebuffer_smoke
