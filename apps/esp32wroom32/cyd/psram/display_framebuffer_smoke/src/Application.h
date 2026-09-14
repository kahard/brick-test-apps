#pragma once

#include "FramebufferTest.h"

namespace cyd_display_framebuffer_smoke
{

class Application final
{
public:
    bool initialize();
    void update();

private:
    Board           board_;
    FramebufferTest test_{ board_.display(), board_.time(), board_.logger() };
};

}  // namespace cyd_display_framebuffer_smoke
