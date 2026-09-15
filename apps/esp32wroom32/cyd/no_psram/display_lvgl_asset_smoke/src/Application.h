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
    LvglDemo demo_{ board_.display(), board_.touch(), board_.time(), board_.logger() };
};
}  // namespace cyd_lvgl_asset_smoke
