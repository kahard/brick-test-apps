#include "FramebufferTest.h"

#include <cstddef>

namespace display_framebuffer_smoke
{
namespace
{
    constexpr char          TAG[]       = "brick_st7701s_framebuffer";
    constexpr std::uint32_t kIntervalMs = 500;
}  // namespace

FramebufferTest::FramebufferTest(const Services& services)
    : display_(services.display), frame_buffers_(services.frame_buffers), time_(services.time),
      logger_(services.logger), timer_(services.time)
{
}

bool FramebufferTest::initialize()
{
    logger_.info(TAG, "Display initialized, framebuffer count=%u",
                 static_cast<unsigned>(frame_buffers_.frame_buffer_count()));
    if (frame_buffers_.frame_buffer_count() < 2)
        return false;
    timer_.start(kIntervalMs);
    return fill_and_present(0, 0xF800);
}

bool FramebufferTest::fill_and_present(std::uint8_t index, std::uint16_t color)
{
    brick::interfaces::display::WritablePixelBuffer buffer;
    if (!frame_buffers_.get_frame_buffer(index, buffer))
        return false;
    const std::uint64_t start = time_.micros();
    for (std::uint16_t y = 0; y < buffer.height; ++y)
    {
        auto* row = reinterpret_cast<std::uint16_t*>(buffer.data + static_cast<std::size_t>(y) * buffer.stride_bytes);
        for (std::uint16_t x = 0; x < buffer.width; ++x)
            row[x] = color;
    }
    const std::uint64_t fill = time_.micros() - start;
    if (!frame_buffers_.present_frame_buffer(index) || !display_.wait_for_vsync(100))
        return false;
    logger_.info(TAG, "framebuffer=%u color=0x%04X fill=%lldus present+vsync=%lldus", static_cast<unsigned>(index),
                 static_cast<unsigned>(color), static_cast<long long>(fill),
                 static_cast<long long>(time_.micros() - start));
    return true;
}

void FramebufferTest::update()
{
    if (!timer_.expired())
        return;
    timer_.restart();
    index_ ^= 1U;
    if (!fill_and_present(index_, index_ == 0 ? 0xF800 : 0x001F))
        logger_.error(TAG, "Frame presentation failed");
}
}  // namespace display_framebuffer_smoke
