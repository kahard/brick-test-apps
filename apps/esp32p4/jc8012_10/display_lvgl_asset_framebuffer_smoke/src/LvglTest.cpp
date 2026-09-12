#include "LvglTest.h"

LvglTest::LvglTest(brick::interfaces::display::IDisplayDevice& display, brick::interfaces::display::ITouchscreen& touch,
                   brick::interfaces::time::ITimeProvider& time, brick::interfaces::logging::ILogger& logger)
    : runtime_(display, time), view_(assets_, logger), touch_adapter_(touch)
{
}

bool LvglTest::initialize()
{
    if (!assets_.initialize() || !runtime_.initialize() || touch_adapter_.create() == nullptr)
        return false;
    view_.create();
    lv_refr_now(runtime_.display());
    return true;
}
void LvglTest::update()
{
    runtime_.update();
}
