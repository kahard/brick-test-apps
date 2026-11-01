#pragma once

#include <array>
#include <cstdint>

#include "Services.h"
#include "Types.h"

namespace esp12f_st7789_button_smoke
{
class DisplayButtonTest final
{
public:
    DisplayButtonTest(DisplayDevice& display, Button& button, TimeProvider& time, Logger& logger);
    Status initialize();
    void   update();

private:
    Status draw_rows(std::uint16_t y, std::uint16_t height, std::uint16_t color);
    Status draw_pattern();
    Status draw_button_state(bool pressed);

    DisplayDevice&                            display_;
    Button&                                   button_;
    TimeProvider&                             time_;
    Logger&                                   logger_;
    std::array<std::uint8_t, 240U * 40U * 2U> pixels_{};
    bool                                      last_pressed_ = false;
    std::uint32_t                             next_poll_ms_ = 0;
};
}  // namespace esp12f_st7789_button_smoke
