#include "FramebufferTest.h"

#include "brick/interfaces/display/PixelBuffer.h"

namespace cyd_display_framebuffer_smoke
{
namespace
{
    constexpr char          kTag[]           = "brick_cyd_framebuffer";
    constexpr std::uint16_t kWidth           = 320;
    constexpr std::uint16_t kHeight          = 240;
    constexpr std::size_t   kFrameBytes      = static_cast<std::size_t>(kWidth) * kHeight * 2;
    constexpr std::uint32_t kColorIntervalMs = 500;
}  // namespace

FramebufferTest::FramebufferTest(const Services& services)
    : display_(services.display), time_(services.time), logger_(services.logger),
      frames_{ brick::core::memory::MemoryBlock{ services.memory },
               brick::core::memory::MemoryBlock{ services.memory } }
{
}

bool FramebufferTest::initialize()
{
    if (!frames_[0].allocate(kFrameBytes, brick::interfaces::memory::MemoryKind::external)
        || !frames_[1].allocate(kFrameBytes, brick::interfaces::memory::MemoryKind::external))
    {
        logger_.error(kTag, "PSRAM framebuffer allocation failed");
        return false;
    }

    logger_.info(kTag, "PSRAM framebuffer test ready: frame=%u bytes, buffers=2", static_cast<unsigned>(kFrameBytes));
    present_next_color();
    next_color_ms_ = time_.millis() + kColorIntervalMs;
    return true;
}

void FramebufferTest::update()
{
    // Signed difference: still correct when millis() wraps after ~49.7 days.
    if (static_cast<std::int32_t>(time_.millis() - next_color_ms_) < 0)
        return;
    present_next_color();
    next_color_ms_ = time_.millis() + kColorIntervalMs;
}

void FramebufferTest::fill_frame(std::uint8_t* framebuffer, std::uint16_t color)
{
    for (std::size_t offset = 0; offset < kFrameBytes; offset += 2)
    {
        framebuffer[offset]     = static_cast<std::uint8_t>(color & 0xFFU);
        framebuffer[offset + 1] = static_cast<std::uint8_t>(color >> 8U);
    }
}

bool FramebufferTest::present(const std::uint8_t* framebuffer, std::uint64_t& transfer_us)
{
    const std::uint64_t                           started = time_.micros();
    const brick::interfaces::display::PixelBuffer buffer{ framebuffer,
                                                          kWidth,
                                                          kHeight,
                                                          static_cast<std::size_t>(kWidth) * 2,
                                                          brick::interfaces::display::PixelFormat::rgb565,
                                                          false };
    if (!display_.draw_buffer({ 0, 0, kWidth, kHeight }, buffer))
        return false;
    transfer_us = time_.micros() - started;
    return true;
}

void FramebufferTest::present_next_color()
{
    constexpr std::uint16_t colors[]    = { 0xF800, 0x07E0, 0x001F, 0xFFE0, 0xF81F, 0x07FF };
    constexpr std::size_t   color_count = sizeof(colors) / sizeof(colors[0]);
    fill_frame(frames_[0].data(), colors[color_index_]);
    std::uint64_t transfer_us = 0;
    if (!present(frames_[0].data(), transfer_us))
    {
        logger_.error(kTag, "SPI transfer failed");
        return;
    }

    transfer_total_us_ += transfer_us;
    ++measured_frames_;
    color_index_ = static_cast<std::uint8_t>((color_index_ + 1U) % color_count);
    if (measured_frames_ == color_count)
    {
        // Integer arithmetic only: fps is reported with two decimals.
        const std::uint64_t average_us = transfer_total_us_ / measured_frames_;
        const std::uint64_t fps_x100   = average_us > 0 ? 100000000ULL / average_us : 0;
        logger_.info(kTag, "SPI full-frame transfer=%lluus (%llu.%02llu fps)",
                     static_cast<unsigned long long>(average_us), static_cast<unsigned long long>(fps_x100 / 100),
                     static_cast<unsigned long long>(fps_x100 % 100));
        measured_frames_   = 0;
        transfer_total_us_ = 0;
    }
}

}  // namespace cyd_display_framebuffer_smoke
