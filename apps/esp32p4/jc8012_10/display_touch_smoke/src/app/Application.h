#pragma once

#include "TouchTest.h"
#include "Types.h"

namespace display_touch_smoke
{
/// Portable application; the board is started by the target entry point.
class Application final
{
public:
    explicit Application(const Services& services);

    bool initialize();
    void update();

private:
    TouchTest test_;
};
}  // namespace display_touch_smoke
