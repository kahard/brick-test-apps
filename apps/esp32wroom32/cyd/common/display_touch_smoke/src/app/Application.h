#pragma once

#include "Services.h"
#include "TouchTest.h"

namespace cyd_display_touch_smoke
{
/// Display and touch smoke test: draws a colour pattern and marks every touch.
/// Runs unchanged on the CYD and in the PC simulator; the board is started by
/// the target entry point before initialize().
class Application final
{
public:
    explicit Application(const Services& services);

    bool initialize();
    void update();

private:
    Services  services_;
    TouchTest test_;
};
}  // namespace cyd_display_touch_smoke
