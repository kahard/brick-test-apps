#pragma once

#include "FramebufferTest.h"
#include "Services.h"

namespace cyd_display_framebuffer_smoke
{

/// Portable application; the board is started by the target entry point.
class Application final
{
public:
    explicit Application(const Services& services);

    bool initialize();
    void update();

private:
    FramebufferTest test_;
};

}  // namespace cyd_display_framebuffer_smoke
