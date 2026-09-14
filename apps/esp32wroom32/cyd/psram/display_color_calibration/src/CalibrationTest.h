#pragma once

#include <cstdint>

#include "CalibrationBuffer.h"
#include "CalibrationImage.h"
#include "CalibrationProtocol.h"
#include "brick/core/display/Rgb565ColorProfile.h"
#include "brick/interfaces/display/IBacklight.h"
#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/display/ITouchscreen.h"
#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/time/ITimeProvider.h"

namespace cyd_color_calibration
{

class CalibrationTest final
{
public:
    CalibrationTest(brick::interfaces::display::IDisplayDevice& display,
                    brick::interfaces::display::IBacklight& backlight, brick::interfaces::time::ITimeProvider& time,
                    brick::interfaces::logging::ILogger& logger, brick::interfaces::display::ITouchscreen& touch);

    bool initialize();
    void update();

private:
    bool          render();
    bool          render_image_stripe(std::uint8_t stripe_index, const std::uint8_t* pixels);
    bool          render_cached_image();
    std::uint16_t source_pixel(std::uint16_t x, std::uint16_t y) const;
    void          apply_pending_commands();
    void          update_image_divider();

    brick::interfaces::display::IDisplayDevice& display_;
    brick::interfaces::display::IBacklight&     backlight_;
    brick::interfaces::time::ITimeProvider&     time_;
    brick::interfaces::logging::ILogger&        logger_;
    brick::interfaces::display::ITouchscreen&   touch_;
    CalibrationBuffer                           buffer_;
    CalibrationImage                            image_;
    CalibrationProtocol                         protocol_;
    brick::core::display::Rgb565ColorProfile    profile_        = brick::core::display::Rgb565ColorProfile::identity();
    std::uint8_t                                pattern_        = 0U;
    std::uint16_t                               divider_x_      = CalibrationImage::width() / 2U;
    std::int16_t                                last_touch_x_   = 0;
    std::uint32_t                               last_tap_ms_    = 0U;
    bool                                        touch_dragging_ = false;
    bool                                        showing_custom_image_ = false;
};

}  // namespace cyd_color_calibration
