#pragma once

#include "LvglDemo.h"
#include "Types.h"

namespace cyd_lvgl_asset_smoke
{
class Application final
{
public:
    bool initialize();
    void update();

private:
    Board    board_;
    LvglDemo demo_{ board_ };
};
}  // namespace cyd_lvgl_asset_smoke
