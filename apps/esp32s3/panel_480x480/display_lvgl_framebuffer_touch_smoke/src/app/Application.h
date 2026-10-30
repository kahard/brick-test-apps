#pragma once

#include "LvglTest.h"
#include "Services.h"

namespace lvgl_framebuffer_touch_smoke
{
/// Portable application; the board is started by the target entry point.
class Application final
{
public:
    explicit Application(const Services& services);

    bool initialize();
    void update();

private:
    LvglTest test_;
};
}  // namespace lvgl_framebuffer_touch_smoke
