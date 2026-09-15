#include "SpiBenchmarkTest.h"
#include "assets/joy_tears_240_rgb565.h"
#include "assets/sweat_smile_240_rgb565.h"
#include "brick/interfaces/display/PixelBuffer.h"
#include <pgmspace.h>
namespace esp12f_st7789_spi_benchmark_smoke
{
SpiBenchmarkTest::SpiBenchmarkTest(DisplayDevice& d, TimeProvider& t, Logger& l) : display_(d), time_(t), logger_(l)
{
}
bool SpiBenchmarkTest::initialize()
{
    start_ = time_.millis();
    return true;
}
bool SpiBenchmarkTest::stream(const brick_image_asset_t& a)
{
    for (std::uint16_t y = 0; y < 240; y += BRICK_STREAM_STRIPE_HEIGHT)
    {
        memcpy_P(stripe_.data(), a.data + static_cast<std::size_t>(y) * 480, stripe_.size());
        brick::interfaces::display::PixelBuffer b{
            stripe_.data(), 240, BRICK_STREAM_STRIPE_HEIGHT, 480, brick::interfaces::display::PixelFormat::rgb565, false
        };
        if (!display_.draw_buffer({ 0, y, 240, BRICK_STREAM_STRIPE_HEIGHT }, b))
            return false;
    }
    return true;
}
void SpiBenchmarkTest::update()
{
    const auto& a = (frames_ & 1U) ? brick_sweat_smile_240 : brick_joy_tears_240;
    const auto  s = time_.micros();
    if (!stream(a))
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
