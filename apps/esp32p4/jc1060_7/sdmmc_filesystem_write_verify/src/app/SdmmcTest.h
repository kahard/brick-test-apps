#pragma once

#include "Services.h"
#include <array>
#include <cstdint>

#include "Types.h"
#include "brick/core/display/Screen.h"
#include "brick/core/time/Timer.h"

class SdmmcTest final
{
public:
    explicit SdmmcTest(const Services& services);

    bool initialize();
    void update();

private:
    void show_status(std::uint16_t color, const char* message);
    bool write_read_verify();

    Services services_;
    brick::core::display::Screen screen_;
    // Pixels of every canvas, sized for the largest status strip.
    std::array<std::uint16_t, 1024U * 32U> canvas_pixels_{};
    brick::core::time::Timer touch_timer_;
    std::array<brick::interfaces::display::TouchPoint, 5> points_{};
    bool touch_was_down_ = false;
};
