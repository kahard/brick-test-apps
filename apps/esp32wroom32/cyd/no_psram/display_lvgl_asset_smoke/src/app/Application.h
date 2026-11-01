#pragma once

#include "LvglDemo.h"
#include "Services.h"

namespace cyd_lvgl_asset_smoke
{
/// Portable application; the board is started by the target entry point.
class Application final
{
public:
    explicit Application(const Services& services);

    Status initialize();
    void   update();

private:
    LvglDemo demo_;
};
}  // namespace cyd_lvgl_asset_smoke
