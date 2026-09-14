#include "Application.h"

namespace cyd_color_calibration
{

bool Application::initialize()
{
    if (!board_.begin())
    {
        board_.logger().error("brick_cyd_calibration", "CYD initialization failed");
        return false;
    }
    return test_.initialize();
}

void Application::update()
{
    test_.update();
}

}  // namespace cyd_color_calibration
