#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "NetworkServices.h"
#include "brick/core/display/Screen.h"

namespace network_test
{

/// The screen as lines of text in the application's font
/// (generated/network_font.h); a line is redrawn only when its text or colour
/// changes, so show() can be called on every update.
class TextLines final
{
public:
    static constexpr std::size_t kMaxLines   = 48;
    static constexpr std::size_t kLineLength = 72;

    /// pixels: one line, screen width x line_height.
    TextLines(brick::interfaces::display::IDisplayDevice& display, std::int32_t line_height, std::uint16_t* pixels,
              std::size_t capacity);

    /// Lines that fit on the screen.
    std::size_t count() const { return count_; }
    /// Clears the screen and forgets what was shown.
    Status clear();
    /// Shows text on line index (printf format, a constant). Lines past
    /// count() are ignored; a drawing failure is kept for take_failure().
    void show(std::size_t index, std::uint16_t color, const char* format, ...) __attribute__((format(printf, 4, 5)));
    /// The first drawing failure since the last call (Status::ok: none).
    Status take_failure();

private:
    Status draw_(std::size_t index);

    brick::core::display::Screen                         screen_;
    std::int32_t                                         line_height_;
    std::uint16_t*                                       pixels_;
    std::size_t                                          capacity_;
    std::size_t                                          count_;
    std::array<std::array<char, kLineLength>, kMaxLines> texts_{};
    std::array<std::uint16_t, kMaxLines>                 colors_{};
    std::array<bool, kMaxLines>                          drawn_{};
    Status                                               failure_ = Status::ok;
};

}  // namespace network_test
