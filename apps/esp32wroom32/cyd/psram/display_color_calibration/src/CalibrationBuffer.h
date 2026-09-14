#pragma once

#include <cstddef>
#include <cstdint>

namespace cyd_color_calibration
{

class CalibrationBuffer final
{
public:
    ~CalibrationBuffer();

    bool           initialize();
    std::uint16_t* pixels() const;

    static constexpr std::uint16_t width() { return kWidth; }
    static constexpr std::uint16_t height() { return kHeight; }

private:
    static constexpr std::uint16_t kWidth      = 320U;
    static constexpr std::uint16_t kHeight     = 16U;
    static constexpr std::size_t   kPixelCount = kWidth * kHeight;
    std::uint16_t*                 pixels_     = nullptr;
};

}  // namespace cyd_color_calibration
