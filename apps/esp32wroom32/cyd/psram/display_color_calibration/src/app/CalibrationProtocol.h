#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "Services.h"
#include "brick/core/display/Rgb565ColorProfile.h"

namespace cyd_color_calibration
{

/// Parses the frames of tools/color_calibration_gui.py from the serial port:
/// 0xA5, command, payload.
class CalibrationProtocol final
{
public:
    explicit CalibrationProtocol(SerialPort& serial) : serial_(serial) {}

    void poll();

    bool consume_brightness_percent(std::uint8_t& percent);
    bool consume_pattern(std::uint8_t& pattern);
    bool consume_profile(brick::core::display::Rgb565ColorProfile& profile);
    bool consume_image_stripe(std::uint8_t& stripe_index, const std::uint8_t*& pixels);

private:
    static constexpr std::uint8_t kFrameStart         = 0xA5U;
    static constexpr std::uint8_t kBrightnessCommand  = 1U;
    static constexpr std::uint8_t kProfileCommand     = 2U;
    static constexpr std::uint8_t kPatternCommand     = 3U;
    static constexpr std::uint8_t kImageStripeCommand = 4U;
    static constexpr std::size_t  kImageStripeBytes   = 320U * 16U * 2U;

    void process_byte(std::uint8_t byte);
    void complete_frame();
    void reset_parser();

    SerialPort&                                      serial_;
    std::array<std::uint8_t, 512U>                   received_{};
    std::size_t                                      received_begin_ = 0U;
    std::size_t                                      received_end_   = 0U;
    std::array<std::uint8_t, 1U + kImageStripeBytes> payload_{};
    std::size_t                                      payload_size_     = 0U;
    std::size_t                                      expected_size_    = 0U;
    std::uint8_t                                     command_          = 0U;
    bool                                             waiting_for_type_ = false;
    bool                                             brightness_ready_ = false;
    bool                                             pattern_ready_    = false;
    bool                                             profile_ready_    = false;
    bool                                             image_ready_      = false;
    std::uint8_t                                     brightness_       = 100U;
    std::uint8_t                                     pattern_          = 0U;
    brick::core::display::Rgb565ColorProfile         profile_ = brick::core::display::Rgb565ColorProfile::identity();
};

}  // namespace cyd_color_calibration
