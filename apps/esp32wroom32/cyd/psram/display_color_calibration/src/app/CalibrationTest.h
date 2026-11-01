#pragma once

#include <cstdint>

#include "CalibrationBuffer.h"
#include "CalibrationImage.h"
#include "CalibrationProtocol.h"
#include "Services.h"
#include "brick/core/display/Rgb565ColorProfile.h"
#include "brick/core/input/DragSlider.h"

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
    void          apply_brightness();
    bool          apply_profile();
    bool          apply_pattern();
    void          apply_image_stripe();
    void          update_image_divider();

    DisplayDevice&                           display_;
    Backlight&                               backlight_;
    TimeProvider&                            time_;
    Logger&                                  logger_;
    Touchscreen&                             touch_;
    CalibrationBuffer                        buffer_;
    CalibrationImage                         image_;
    CalibrationProtocol                      protocol_;
    brick::core::input::DragSlider           divider_;
    brick::core::display::Rgb565ColorProfile profile_ = brick::core::display::Rgb565ColorProfile::identity();
    std::uint8_t                             pattern_ = 0U;
    bool                                     showing_custom_image_ = false;
};

}  // namespace cyd_color_calibration
