#pragma once

#include "DisplayButtonTest.h"

namespace esp12f_st7789_button_smoke
{
class Application final
{
public:
    bool initialize();
    void update();

private:
    Board             board_;
    DisplayButtonTest test_{ board_.display(), board_.button(), board_.time(), board_.logger() };
};
}  // namespace esp12f_st7789_button_smoke
