#pragma once
#include "Types.h"
#include "assets/joy_tears_240_rgb565.h"
#include <array>
namespace esp12f_st7789_spi_benchmark_smoke
{
class SpiBenchmarkTest final
{
public:
    SpiBenchmarkTest(DisplayDevice&, TimeProvider&, Logger&);
    bool initialize();
    void update();

private:
    bool                                                             stream(const brick_image_asset_t&);
    DisplayDevice&                                                   display_;
    TimeProvider&                                                    time_;
    Logger&                                                          logger_;
    std::array<std::uint8_t, 240U * BRICK_STREAM_STRIPE_HEIGHT * 2U> stripe_{};
    std::uint32_t                                                    frames_ = 0;
    std::uint32_t                                                    start_  = 0;
};
}  // namespace esp12f_st7789_spi_benchmark_smoke
