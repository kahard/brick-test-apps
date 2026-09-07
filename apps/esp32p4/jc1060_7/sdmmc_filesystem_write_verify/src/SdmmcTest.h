#pragma once

#include <array>
#include <cstdint>

#include "Types.h"
#include "brick/core/display/Screen.h"
#include "brick/core/time/Timer.h"

class SdmmcTest final
{
public:
    explicit SdmmcTest(Board& board);

    bool initialize();
    void update();

private:
    void show_status(std::uint16_t color, const char* message);
    bool write_read_verify();

    Board& board_;
    brick::core::display::Screen screen_;
    brick::core::time::Timer touch_timer_;
    std::array<brick::interfaces::display::TouchPoint, 5> points_{};
    bool touch_was_down_ = false;
};
