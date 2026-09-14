#include "CalibrationProtocol.h"

#include <cstring>

#include "driver/uart.h"
#include "esp_err.h"

namespace cyd_color_calibration
{

bool CalibrationProtocol::initialize()
{
    constexpr std::size_t receive_buffer_bytes = 20U * 1024U;
    const esp_err_t       result = uart_driver_install(UART_NUM_0, receive_buffer_bytes, 0U, 0U, nullptr, 0U);

    // UART0 can already be owned by the ESP-IDF console. In that case the
    // existing driver is also the one used by uart_read_bytes().
    return result == ESP_OK || result == ESP_ERR_INVALID_STATE;
}

void CalibrationProtocol::poll()
{
    std::array<std::uint8_t, 512U> received{};
    const int                      bytes_read = uart_read_bytes(UART_NUM_0, received.data(), received.size(), 0U);
    for (int index = 0; index < bytes_read; ++index)
        process_byte(received[static_cast<std::size_t>(index)]);
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
