#pragma once

#include <array>

#include "brick/core/display/Screen.h"
#include "brick/core/time/Timer.h"
#include "brick/interfaces/display/ITouchscreen.h"
#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/storage/IFileSystem.h"

namespace cyd_sd_card_filesystem_touch_smoke
{
class SdCardTest final
{
public:
    SdCardTest(brick::interfaces::display::IDisplayDevice& display, brick::interfaces::storage::IFileSystem& filesystem,
               brick::interfaces::display::ITouchscreen& touch, brick::interfaces::time::ITimeProvider& time,
               brick::interfaces::logging::ILogger& logger);
    bool initialize();
    void update();

private:
    void clear_screen();
    void show_status(std::uint16_t color, const char* message);
    bool write_read_verify();
    void refresh_card_status();

    brick::interfaces::storage::IFileSystem&             filesystem_;
    brick::interfaces::display::ITouchscreen&            touch_;
    brick::interfaces::time::ITimeProvider&              time_;
    brick::interfaces::logging::ILogger&                 logger_;
    brick::core::display::Screen                          screen_;
    brick::core::time::Timer                              card_timer_;
    std::array<brick::interfaces::display::TouchPoint, 5> points_{};
    bool                                                  touch_was_down_ = false;
};
}  // namespace cyd_sd_card_filesystem_touch_smoke
