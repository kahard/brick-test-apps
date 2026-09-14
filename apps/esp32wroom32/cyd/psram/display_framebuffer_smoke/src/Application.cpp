#include "Application.h"

namespace cyd_display_framebuffer_smoke
{

bool Application::initialize()
{
    if (!board_.begin())
    {
        board_.logger().error("brick_cyd_framebuffer", "CYD display initialization failed");
        return false;
    }
    return test_.initialize();
}

void Application::update()
{
    test_.update();
    board_.time().delay_ms(1);
}

}  // namespace cyd_display_framebuffer_smoke
