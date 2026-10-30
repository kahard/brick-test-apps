#include "CalibrationProtocol.h"

#include <cstring>

namespace cyd_color_calibration
{

void CalibrationProtocol::poll()
{
    if (received_begin_ == received_end_)
    {
        received_begin_ = 0U;
        received_end_   = serial_.read(received_.data(), received_.size());
    }
    // An image stripe is handed out straight from payload_, so stop at a
    // complete one; the following frame would overwrite it before it is
    // consumed. The remaining bytes are parsed by the next poll().
    while (received_begin_ < received_end_ && !image_ready_)
        process_byte(received_[received_begin_++]);
}

bool CalibrationProtocol::consume_brightness_percent(std::uint8_t& percent)
{
    if (!brightness_ready_)
        return false;
    brightness_ready_ = false;
    percent           = brightness_;
    return true;
}

bool CalibrationProtocol::consume_pattern(std::uint8_t& pattern)
{
    if (!pattern_ready_)
        return false;
    pattern_ready_ = false;
    pattern        = pattern_;
    return true;
}

bool CalibrationProtocol::consume_profile(brick::core::display::Rgb565ColorProfile& profile)
{
    if (!profile_ready_)
        return false;
    profile_ready_ = false;
    profile        = profile_;
    return true;
}

bool CalibrationProtocol::consume_image_stripe(std::uint8_t& stripe_index, const std::uint8_t*& pixels)
{
    if (!image_ready_)
        return false;
    image_ready_ = false;
    stripe_index = payload_[0];
    pixels       = payload_.data() + 1U;
    return true;
}

void CalibrationProtocol::process_byte(std::uint8_t byte)
{
    if (expected_size_ == 0U && byte == kFrameStart)
    {
        waiting_for_type_ = true;
        payload_size_     = 0U;
        expected_size_    = 0U;
        return;
    }
    if (waiting_for_type_)
    {
        waiting_for_type_ = false;
        command_          = byte;
        expected_size_    = byte == kProfileCommand     ? brick::core::display::Rgb565ColorProfile::serialized_size :
                            byte == kImageStripeCommand ? payload_.size() :
                                                          1U;
        if (byte != kBrightnessCommand && byte != kProfileCommand && byte != kPatternCommand
            && byte != kImageStripeCommand)
            reset_parser();
        return;
    }
    if (expected_size_ == 0U || payload_size_ >= expected_size_)
        return;

    payload_[payload_size_++] = byte;
    if (payload_size_ == expected_size_)
        complete_frame();
}

void CalibrationProtocol::complete_frame()
{
    if (command_ == kBrightnessCommand && payload_[0] <= 100U)
    {
        brightness_       = payload_[0];
        brightness_ready_ = true;
    }
    else if (command_ == kPatternCommand)
    {
        pattern_       = payload_[0];
        pattern_ready_ = true;
    }
    else if (command_ == kProfileCommand)
    {
        std::memcpy(profile_.red.data(), payload_.data(), profile_.red.size());
        std::memcpy(profile_.green.data(), payload_.data() + profile_.red.size(), profile_.green.size());
        std::memcpy(profile_.blue.data(), payload_.data() + profile_.red.size() + profile_.green.size(),
                    profile_.blue.size());
        profile_ready_ = profile_.valid();
    }
    else if (command_ == kImageStripeCommand && payload_[0] < 15U)
        image_ready_ = true;
    reset_parser();
}

void CalibrationProtocol::reset_parser()
{
    payload_size_     = 0U;
    expected_size_    = 0U;
    command_          = 0U;
    waiting_for_type_ = false;
}

}  // namespace cyd_color_calibration
