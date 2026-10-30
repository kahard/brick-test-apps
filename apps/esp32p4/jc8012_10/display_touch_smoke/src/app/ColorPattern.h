#pragma once

#include "brick/interfaces/display/WritablePixelBuffer.h"

namespace display_touch_smoke
{
class ColorPattern final
{
public:
    void fill(brick::interfaces::display::WritablePixelBuffer& pixels) const;

    static std::uint16_t background_color(std::uint32_t x, std::uint32_t y, std::uint32_t height);
};
}  // namespace display_touch_smoke
