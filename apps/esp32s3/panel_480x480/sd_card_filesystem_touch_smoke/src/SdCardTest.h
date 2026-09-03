#pragma once

#include <array>

#include "ApplicationConfig.h"
#include "brick/boards/esp32/s3/Panel480BoardTemplate.h"
#include "brick/core/display/Screen.h"
#include "brick/core/time/Timer.h"

class SdCardTest final
{
public:
    using Board = brick::platform::esp32::s3::Panel480BoardTemplate<SdFeatures>;

    explicit SdCardTest(Board& board);
    bool initialize();
    void update();

private:
    void show_status(std::uint16_t color, const char* message);
    bool write_read_verify();

    Board&                                                board_;
    brick::core::display::Screen                          screen_;
    brick::core::time::Timer                              card_timer_;
    std::array<brick::interfaces::display::TouchPoint, 5> points_{};
    bool                                                  touch_was_down_ = false;
};
