#include "FramebufferTest.h"

FramebufferTest::FramebufferTest(brick::interfaces::display::IDisplayDevice& display,
                                 brick::interfaces::time::ITimeProvider&     time,
                                 brick::interfaces::logging::ILogger&        logger)
    : buffers_(display), screen_(display), time_(time), logger_(logger), timer_(time)
{
}

bool FramebufferTest::initialize()
{
    if (!buffers_.initialize() || !draw_next())
        return false;
    timer_.start(500);
    return true;
}

bool FramebufferTest::draw_next()
{
    const std::uint16_t                             color   = index_ == 0 ? 0xF800 : 0x001F;
    brick::interfaces::display::WritablePixelBuffer pixels  = buffers_.buffer(index_);
    const std::uint64_t                             started = time_.micros();
    if (!screen_.fill(pixels, color))
        return false;
    const std::uint64_t filled = time_.micros();
    if (!buffers_.present(index_))
        return false;
    logger_.info("framebuffer", "render buffer=%u color=0x%04X fill=%lluus submit+refresh=%lluus",
                 static_cast<unsigned>(index_), static_cast<unsigned>(color),
                 static_cast<unsigned long long>(filled - started),
                 static_cast<unsigned long long>(time_.micros() - filled));
    index_ ^= 1U;
    return true;
}

void FramebufferTest::update()
{
    if (failed_ || !timer_.expired())
        return;
    timer_.restart();
    if (!draw_next())
    {
        logger_.error("framebuffer", "Frame presentation failed");
        failed_ = true;
    }
}
