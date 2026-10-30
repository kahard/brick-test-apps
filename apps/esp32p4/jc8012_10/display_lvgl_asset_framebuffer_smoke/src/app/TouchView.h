#pragma once

#include <cstdint>

#include "LvglAssets.h"
#include "Services.h"

namespace lvgl_asset_smoke
{
/// Title, one bundle image and a button; each click switches the image.
class TouchView final
{
public:
    TouchView(LvglAssets& assets, Logger& logger);
    void create();

private:
    static void   clicked(lv_event_t* event);
    void          change_image();
    LvglAssets&   assets_;
    Logger&       logger_;
    lv_obj_t*     image_  = nullptr;
    lv_obj_t*     status_ = nullptr;
    std::uint32_t clicks_ = 0;
};
}  // namespace lvgl_asset_smoke
