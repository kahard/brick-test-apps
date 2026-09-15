#pragma once

#include "FontTest.h"

namespace esp12f_st7789_display_font_smoke
{
class Application final
{
public:
    bool initialize();
    void update();

private:
    Board    board_;
    FontTest test_{ board_.display(), board_.button() };
};
}  // namespace esp12f_st7789_display_font_smoke
