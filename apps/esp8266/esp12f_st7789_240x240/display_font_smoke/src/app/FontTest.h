#pragma once

#include "Types.h"
#include "brick/core/display/Screen.h"
#include "brick/interfaces/display/WritablePixelBuffer.h"

struct BrickBitmapGlyph;

namespace esp12f_st7789_display_font_smoke
{
class FontTest final
{
public:
    FontTest(brick::interfaces::display::IDisplayDevice& display, brick::interfaces::input::IButton& button)
        : screen_(display), button_(button) {}
    bool initialize();
    void update();
private:
    bool clear_display();
    bool draw_page();
    bool draw_line(std::int32_t y, std::int32_t height, const char* value, const char* characters,
                   const BrickBitmapGlyph* glyphs, std::size_t glyph_count, std::uint16_t color);
    static void draw_progmem_text(brick::interfaces::display::WritablePixelBuffer buffer, std::int32_t x,
                                  std::int32_t y, const char* value, const char* characters,
                                  const BrickBitmapGlyph* glyphs, std::size_t glyph_count, std::uint16_t color);
    static void set_pixel(brick::interfaces::display::WritablePixelBuffer buffer, std::int32_t x,
                          std::int32_t y, std::uint16_t color);

    brick::core::display::Screen screen_;
    brick::interfaces::input::IButton& button_;
    bool pressed_ = false;
    bool alternate_ = false;
};
}
