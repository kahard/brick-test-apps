#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace cyd_color_calibration
{

class CalibrationImage final
{
public:
    static constexpr std::uint16_t kWidth        = 320U;
    static constexpr std::uint16_t kHeight       = 240U;
    static constexpr std::uint16_t kStripeHeight = 16U;
    static constexpr std::uint8_t  kStripeCount  = kHeight / kStripeHeight;

    ~CalibrationImage();

    bool initialize();
    bool store_stripe(std::uint8_t stripe_index, const std::uint8_t* pixels);

    bool                complete() const;
    const std::uint8_t* stripe(std::uint8_t stripe_index) const;

    static constexpr std::uint16_t width() { return kWidth; }
    static constexpr std::uint16_t height() { return kHeight; }
    static constexpr std::uint16_t stripe_height() { return kStripeHeight; }
    static constexpr std::uint8_t  stripe_count() { return kStripeCount; }
    static constexpr std::size_t   stripe_size_bytes()
    {
        return static_cast<std::size_t>(width()) * stripe_height() * sizeof(std::uint16_t);
    }

private:
    static constexpr std::size_t kImageSizeBytes = static_cast<std::size_t>(kWidth) * kHeight * sizeof(std::uint16_t);

    std::uint8_t*                  pixels_ = nullptr;
    std::array<bool, kStripeCount> received_{};
};

}  // namespace cyd_color_calibration
