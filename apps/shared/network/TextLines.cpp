#include "TextLines.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>

#include "generated/network_font.h"

namespace network_test
{

namespace
{
    constexpr std::uint16_t kBackground = 0x0000;
    constexpr std::int32_t  kMarginX    = 6;
    /// Space above the glyphs: an eighth of the line.
    constexpr std::int32_t kTopDivisor = 8;
}  // namespace

TextLines::TextLines(brick::interfaces::display::IDisplayDevice& display, std::int32_t line_height,
                     std::uint16_t* pixels, std::size_t capacity)
    : screen_(display), line_height_(line_height > 0 ? line_height : 1), pixels_(pixels), capacity_(capacity)
{
    const std::size_t lines = static_cast<std::size_t>(screen_.size().height) / static_cast<std::size_t>(line_height_);
    count_                  = lines < kMaxLines ? lines : kMaxLines;
}

Status TextLines::clear()
{
    const auto size = screen_.size();
    drawn_.fill(false);
    return screen_.fill_area({ 0, 0, static_cast<std::int32_t>(size.width), static_cast<std::int32_t>(size.height) },
                             kBackground, pixels_, capacity_);
}

void TextLines::show(std::size_t index, std::uint16_t color, const char* format, ...)
{
    if (index >= count_)
        return;
    std::array<char, kLineLength> text{};
    va_list                       arguments;
    va_start(arguments, format);
    std::vsnprintf(text.data(), text.size(), format, arguments);
    va_end(arguments);
    if (drawn_[index] && colors_[index] == color && std::strcmp(texts_[index].data(), text.data()) == 0)
        return;
    texts_[index]  = text;
    colors_[index] = color;
    if (const Status status = draw_(index); !is_ok(status) && is_ok(failure_))
        failure_ = status;
}

void TextLines::bar(std::size_t index, std::uint16_t color, std::uint32_t permille)
{
    if (index >= count_)
        return;
    const auto         size  = screen_.size();
    const auto         width = static_cast<std::int32_t>(size.width);
    const std::int32_t done  = static_cast<std::int32_t>((permille > 1000 ? 1000 : permille) * size.width / 1000);
    if (!drawn_[index] && bars_[index] == done)
        return;
    const std::int32_t top = static_cast<std::int32_t>(index) * line_height_;
    Status             status =
        done > 0 ? screen_.fill_area({ 0, top, done, line_height_ }, color, pixels_, capacity_) : Status::ok;
    if (is_ok(status) && done < width)
        status = screen_.fill_area({ done, top, width - done, line_height_ }, kBackground, pixels_, capacity_);
    drawn_[index] = false;
    bars_[index]  = is_ok(status) ? done : -1;
    if (!is_ok(status) && is_ok(failure_))
        failure_ = status;
}

Status TextLines::take_failure()
{
    const Status failure = failure_;
    failure_             = Status::ok;
    return failure;
}

Status TextLines::draw_(std::size_t index)
{
    const auto                                    size = screen_.size();
    const brick::interfaces::display::DisplayRect area{ 0, static_cast<std::int32_t>(index) * line_height_,
                                                        static_cast<std::int32_t>(size.width), line_height_ };
    const Status                                  status =
        screen_.create_canvas(area, pixels_, capacity_)
            .clear(kBackground)
            .text(kMarginX, line_height_ / kTopDivisor, texts_[index].data(), brick_network_font_chars,
                  brick_network_font_glyphs, brick_network_font_count, colors_[index])
            .present();
    drawn_[index] = is_ok(status);
    bars_[index]  = -1;
    return status;
}

}  // namespace network_test
