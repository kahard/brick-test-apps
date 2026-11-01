#include "LvglTest.h"

namespace lvgl_asset_smoke
{
LvglTest::LvglTest(const Services& services)
    : runtime_(services.display, services.memory, services.time), assets_(services.assets, services.memory),
      view_(assets_, services.logger), touch_adapter_(services.touch)
{
}

Status LvglTest::initialize()
{
    if (!is_ok(assets_.initialize()) || !is_ok(runtime_.initialize()) || touch_adapter_.create() == nullptr)
        return Status::error;
    view_.create();
    lv_refr_now(runtime_.display());
    return Status::ok;
}

void LvglTest::update()
{
    runtime_.update();
}
}  // namespace lvgl_asset_smoke
