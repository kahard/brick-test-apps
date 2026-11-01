#pragma once

#include <cstdint>

#include "CalibrationBuffer.h"
#include "CalibrationImage.h"
#include "CalibrationProtocol.h"
#include "Services.h"
#include "brick/core/display/Rgb565ColorProfile.h"

namespace cyd_color_calibration
{

/// Applies brightness, test patterns, an uploaded image and RGB565 profiles
/// received over the serial port; on the image, dragging moves the divider
/// between source (left) and corrected (right) pixels.
class CalibrationTest final
{
public:
    explicit CalibrationTest(const Services& services);

    Status initialize();
    void   update();

private:
    Status        render();
    Status        render_image_stripe(std::uint8_t stripe_index, const std::uint8_t* pixels);
    Status        render_cached_image();
    std::uint16_t source_pixel(std::uint16_t x, std::uint16_t y) const;
    void          apply_pending_commands();
    void          update_image_divider();

    DisplayDevice&                           display_;
    Backlight&                               backlight_;
    TimeProvider&                            time_;
    Logger&                                  logger_;
    Touchscreen&                             touch_;
    CalibrationBuffer                        buffer_;
    CalibrationImage                         image_;
    CalibrationProtocol                      protocol_;
    brick::core::display::Rgb565ColorProfile profile_        = brick::core::display::Rgb565ColorProfile::identity();
    std::uint8_t                             pattern_        = 0U;
    std::uint16_t                            divider_x_      = CalibrationImage::width() / 2U;
    std::int16_t                             last_touch_x_   = 0;
    std::uint32_t                            last_tap_ms_    = 0U;
    bool                                     touch_dragging_ = false;
    bool                                     showing_custom_image_ = false;
};

}  // namespace cyd_color_calibration
