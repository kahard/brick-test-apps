#pragma once

#include "LvglTest.h"
#include "Services.h"

namespace lvgl_benchmark_smoke
{
/// Portable application; the board is started by the target entry point.
class Application final
{
public:
    explicit Application(const Services& services);

    Status initialize();
    void   update();

private:
    LvglTest test_;
};
}  // namespace lvgl_benchmark_smoke
