#pragma once

#include "FontTest.h"
#include "Services.h"

namespace esp12f_st7789_display_font_smoke
{
/// Portable application; the board is started by the target entry point.
class Application final
{
public:
    explicit Application(const Services& services);

    Status initialize();
    void   update();

private:
    FontTest test_;
};
}  // namespace esp12f_st7789_display_font_smoke
