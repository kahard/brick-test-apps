#include "SpiBenchmarkTest.h"
#include "assets/joy_tears_240_rgb565.h"
#include "assets/sweat_smile_240_rgb565.h"
#include "brick/core/memory/ProgramMemory.h"
#include "brick/interfaces/display/PixelBuffer.h"
namespace esp12f_st7789_spi_benchmark_smoke
{
namespace
{
    constexpr std::uint32_t kTransferTimeoutMs = 1000;
}
SpiBenchmarkTest::SpiBenchmarkTest(DisplayDevice& d, TimeProvider& t, Logger& l) : display_(d), time_(t), logger_(l)
{
}
Status SpiBenchmarkTest::initialize()
{
    start_ = time_.millis();
    return Status::ok;
}
Status SpiBenchmarkTest::stream(const brick_image_asset_t& a)
{
    // The stripe buffer holds two half stripes: one is read from flash while
    // the display still sends the other (DisplayCapabilities::async_transfer).
    constexpr std::uint16_t kRows      = BRICK_STREAM_STRIPE_HEIGHT / 2;
    constexpr std::size_t   kHalfBytes = 240U * kRows * 2U;
    std::uint8_t* const     halves[2]  = { stripe_.data(), stripe_.data() + kHalfBytes };
    std::uint8_t            half       = 0;
    for (std::uint16_t y = 0; y < 240; y += kRows)
    {
        brick_progmem_copy(halves[half], a.data + static_cast<std::size_t>(y) * 480, kHalfBytes);
        if (!is_ok(display_.wait_for_transfer_complete(kTransferTimeoutMs)))
            return Status::timeout;
        brick::interfaces::display::PixelBuffer b{
            halves[half], 240, kRows, 480, brick::interfaces::display::PixelFormat::rgb565, false
        };
        if (!is_ok(display_.submit_buffer({ 0, y, 240, kRows }, b)))
            return Status::error;
        half ^= 1U;
    }
    return display_.wait_for_transfer_complete(kTransferTimeoutMs);
}
void SpiBenchmarkTest::update()
{
    const auto& a = (frames_ & 1U) ? brick_sweat_smile_240 : brick_joy_tears_240;
    const auto  s = time_.micros();
    if (!is_ok(stream(a)))
    {
        logger_.error("spi", "transfer failed");
        return;
    }
    ++frames_;
    if (frames_ % 60 == 0)
    {
        auto e = time_.millis() - start_;
        logger_.info("spi", "60 frames=%lums fps=%.2f", static_cast<unsigned long>(e), e ? 60000.0 / e : 0.0);
        start_ = time_.millis();
    }
    logger_.debug("spi", "frame=%lu elapsed=%luus", static_cast<unsigned long>(frames_),
                  static_cast<unsigned long>(time_.micros() - s));
}
}  // namespace esp12f_st7789_spi_benchmark_smoke
