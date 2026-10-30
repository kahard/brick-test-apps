#pragma once

#include "LvglRuntime.h"
#include "Services.h"
#include "TouchView.h"
#include "brick/lvgl/LvglTouchAdapter.h"

namespace lvgl_asset_smoke
{
class LvglTest final
{
public:
    explicit LvglTest(const Services& services);
    bool initialize();
    void update();

private:
    LvglRuntime                   runtime_;
    LvglAssets                    assets_;
    TouchView                     view_;
    brick::lvgl::LvglTouchAdapter touch_adapter_;
};
}  // namespace lvgl_asset_smoke
