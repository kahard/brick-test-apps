#pragma once
#include "LvglRuntime.h"
#include "TouchView.h"
#include "brick/lvgl/LvglTouchAdapter.h"

class LvglTest final
{
public:
    LvglTest(brick::interfaces::display::IDisplayDevice& display, brick::interfaces::display::ITouchscreen& touch,
             brick::interfaces::time::ITimeProvider& time, brick::interfaces::logging::ILogger& logger);
    bool initialize();
    void update();

private:
    LvglRuntime                              runtime_;
    LvglAssets                               assets_;
    TouchView                                view_;
    brick::lvgl::LvglTouchAdapter touch_adapter_;
};
