#pragma once

#include "DisplayButtonTest.h"
#include "Services.h"

namespace esp12f_st7789_button_smoke
{
/// Portable application; the board is started by the target entry point.
class Application final
{
public:
    explicit Application(const Services& services);

    bool initialize();
    void update();

private:
    DisplayButtonTest test_;
};
}  // namespace esp12f_st7789_button_smoke
