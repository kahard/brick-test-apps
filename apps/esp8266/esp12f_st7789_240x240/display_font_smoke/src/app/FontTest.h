#pragma once

#include <array>
#include <cstdint>

#include "Services.h"
#include "Types.h"
#include "brick/core/display/Screen.h"
#include "brick/interfaces/display/WritablePixelBuffer.h"

struct BrickBitmapGlyph;

namespace esp12f_st7789_display_font_smoke
{
class FontTest final
{
public:
    FontTest(brick::interfaces::display::IDisplayDevice& display, brick::interfaces::input::IButton& button,
             Logger& logger)
        : screen_(display), button_(button), logger_(logger)
    {
    }
    Status initialize();
    void   update();

private:
    Status      clear_display();
    Status      draw_page();
    Status      draw_line(std::int32_t y, std::int32_t height, const char* value, const char* characters,
                          const BrickBitmapGlyph* glyphs, std::size_t glyph_count, std::uint16_t color);
    static void draw_progmem_text(brick::interfaces::display::WritablePixelBuffer buffer, std::int32_t x,
                                  std::int32_t y, const char* value, const char* characters,
                                  const BrickBitmapGlyph* glyphs, std::size_t glyph_count, std::uint16_t color);
    static void set_pixel(brick::interfaces::display::WritablePixelBuffer buffer, std::int32_t x, std::int32_t y,
                          std::uint16_t color);

    brick::core::display::Screen screen_;
    // Pixels of every canvas, sized for the largest text line.
    std::array<std::uint16_t, 240U * 52U> canvas_pixels_{};
    brick::interfaces::input::IButton&    button_;
    Logger&                               logger_;
    bool                                  pressed_   = false;
    bool                                  alternate_ = false;
};
}  // namespace esp12f_st7789_display_font_smoke
