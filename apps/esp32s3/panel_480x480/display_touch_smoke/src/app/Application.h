#pragma once

#include "TouchTest.h"

namespace display_touch_smoke
{
class Application final
{
public:
    bool initialize();
    void update();

private:
    Board     board_;
    TouchTest test_{ board_.display(), board_.touch(), board_.time(), board_.logger() };
};
}  // namespace display_touch_smoke
