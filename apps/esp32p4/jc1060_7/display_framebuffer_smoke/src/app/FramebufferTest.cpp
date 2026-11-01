#include "FramebufferTest.h"

namespace display_framebuffer_smoke
{
FramebufferTest::FramebufferTest(const Services& services)
    : buffers_(services.display, services.memory), screen_(services.display), time_(services.time),
      logger_(services.logger), timer_(services.time)
{
}

Status FramebufferTest::initialize()
{
    if (!is_ok(buffers_.initialize()))
    {
        logger_.error("framebuffer", "External RAM render buffer allocation failed");
        return Status::error;
    }
    if (!is_ok(draw_next()))
        return Status::error;
    timer_.start(500);
    return Status::ok;
}

Status FramebufferTest::draw_next()
{
    const std::uint16_t                             color   = index_ == 0 ? 0xF800 : 0x001F;
    brick::interfaces::display::WritablePixelBuffer pixels  = buffers_.buffer(index_);
    const std::uint64_t                             started = time_.micros();
    if (!is_ok(screen_.fill(pixels, color)))
        return Status::error;
    const std::uint64_t filled = time_.micros();
    if (!is_ok(buffers_.present(index_)))
        return Status::error;
    logger_.info("framebuffer", "render buffer=%u color=0x%04X fill=%lluus submit+refresh=%lluus",
                 static_cast<unsigned>(index_), static_cast<unsigned>(color),
                 static_cast<unsigned long long>(filled - started),
                 static_cast<unsigned long long>(time_.micros() - filled));
    index_ ^= 1U;
    return Status::ok;
}

void FramebufferTest::update()
{
    if (failed_ || !timer_.expired())
        return;
    timer_.restart();
    if (!is_ok(draw_next()))
    {
        logger_.error("framebuffer", "Frame presentation failed");
        failed_ = true;
    }
}
}  // namespace display_framebuffer_smoke
