#pragma once
#include "FrameBuffers.h"
#include "brick/core/time/Timer.h"
#include "brick/interfaces/logging/ILogger.h"

class FramebufferTest final
{
public:
    FramebufferTest(brick::interfaces::display::IDisplayDevice& display, brick::interfaces::time::ITimeProvider& time,
                    brick::interfaces::logging::ILogger& logger);
    bool initialize();
    void update();

private:
    bool                                    draw_next();
    FrameBuffers                            buffers_;
    brick::core::display::Screen            screen_;
    brick::interfaces::time::ITimeProvider& time_;
    brick::interfaces::logging::ILogger&    logger_;
    brick::core::time::Timer                timer_;
    std::uint8_t                            index_  = 0;
    bool                                    failed_ = false;
};
