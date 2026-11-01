#pragma once

#include "FramebufferTest.h"
#include "Services.h"

namespace display_framebuffer_smoke
{
/// Portable application; the board is started by the target entry point.
class Application final
{
public:
    explicit Application(const Services& services);

    Status initialize();
    void   update();

private:
    FramebufferTest test_;
};
}  // namespace display_framebuffer_smoke
