#include "FontTest.h"
#include "generated/font_12.h"
#include "generated/font_20.h"
#include "generated/font_28.h"

#include <pgmspace.h>

namespace esp12f_st7789_display_font_smoke
{
bool FontTest::initialize()
{
    pressed_ = button_.is_pressed();
    return draw_page();
}

void FontTest::update()
{
    const bool now = button_.is_pressed();
    if (now && !pressed_)
    {
        alternate_ = !alternate_;
        draw_page();
    }
    pressed_ = now;
}

bool FontTest::clear_display()
{
    constexpr std::int32_t stripe_height = 24;
    for (std::int32_t y = 0; y < 240; y += stripe_height)
    {
        auto canvas = screen_.create_canvas({ 0, y, 240, stripe_height });
        if (!canvas || !canvas.clear(0x0000).present())
        {
            return false;
        }
    }
    return true;
}

bool FontTest::draw_line(std::int32_t y, std::int32_t height, const char* value, const char* characters,
                         const BrickBitmapGlyph* glyphs, std::size_t glyph_count, std::uint16_t color)
{
    auto canvas = screen_.create_canvas({ 0, y, 240, height });
    if (!canvas)
    {
        return false;
    }

    canvas.clear(0x0000);
    auto buffer = canvas.buffer();
    draw_progmem_text(buffer, 8, 4, value, characters, glyphs, glyph_count, color);
    return canvas.present();
}

bool FontTest::draw_page()
{
    return clear_display()
           && draw_line(0, 40, alternate_ ? "FONT DEMO B" : "FONT DEMO A", brick_roboto_20_chars,
                        brick_roboto_20_glyphs, brick_roboto_20_count, 0xFFE0)
           && draw_line(48, 28, "Roboto 12: ABC 123", brick_roboto_12_chars, brick_roboto_12_glyphs,
                        brick_roboto_12_count, 0xFFFF)
           && draw_line(84, 40, "Roboto 20: TEST", brick_roboto_20_chars, brick_roboto_20_glyphs,
                        brick_roboto_20_count, 0x07E0)
           && draw_line(132, 52, "Roboto 28", brick_roboto_28_chars, brick_roboto_28_glyphs,
                        brick_roboto_28_count, 0x07FF)
           && draw_line(192, 32, "Button: change page", brick_roboto_12_chars, brick_roboto_12_glyphs,
                        brick_roboto_12_count, 0xF81F);
}

void FontTest::draw_progmem_text(brick::interfaces::display::WritablePixelBuffer buffer, std::int32_t x,
                                 std::int32_t y, const char* value, const char* characters,
                                 const BrickBitmapGlyph* glyphs, std::size_t glyph_count, std::uint16_t color)
{
    for (std::size_t value_index = 0; pgm_read_byte(value + value_index) != '\0'; ++value_index)
    {
        const auto character = static_cast<char>(pgm_read_byte(value + value_index));
        BrickBitmapGlyph glyph{};
        bool found = false;

        for (std::size_t glyph_index = 0; glyph_index < glyph_count; ++glyph_index)
        {
            if (static_cast<char>(pgm_read_byte(characters + glyph_index)) == character)
            {
                memcpy_P(&glyph, glyphs + glyph_index, sizeof(glyph));
                found = true;
                break;
            }
        }

        if (!found)
        {
            x += 4;
            continue;
        }

        for (std::uint16_t row = 0; row < glyph.height; ++row)
        {
            for (std::uint16_t column = 0; column < glyph.width; ++column)
            {
                const auto byte = pgm_read_byte(glyph.data + row * glyph.stride + column / 8);
                if ((byte & (0x80U >> (column % 8))) != 0)
                {
                    set_pixel(buffer, x + column, y + glyph.top + row, color);
                }
            }
        }
        x += glyph.width + 1;
    }
}

void FontTest::set_pixel(brick::interfaces::display::WritablePixelBuffer buffer, std::int32_t x, std::int32_t y,
                         std::uint16_t color)
{
    if (x < 0 || y < 0 || x >= static_cast<std::int32_t>(buffer.width) || y >= static_cast<std::int32_t>(buffer.height))
    {
        return;
    }

    auto* row = reinterpret_cast<std::uint16_t*>(buffer.data + static_cast<std::size_t>(y) * buffer.stride_bytes);
    row[x] = color;
}
}
