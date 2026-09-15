#pragma once

#include "LvglBuffers.h"
#include "PartitionAssets.h"
#include "brick/platform/esp32/LvglDisplayAdapter.h"
#include "brick/platform/esp32/LvglTouchAdapter.h"
#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/time/ITimeProvider.h"
#include "lvgl.h"

namespace cyd_lvgl_asset_smoke
{
class LvglDemo final
{
public:
    LvglDemo(brick::interfaces::display::IDisplayDevice& display, brick::interfaces::display::ITouchscreen& touch,
             brick::interfaces::time::ITimeProvider& time, brick::interfaces::logging::ILogger& logger);
    bool initialize();
    void update();

private:
    static void on_button_clicked(lv_event_t* event);
    bool        load_selected_asset();
    void        create_widgets();

    brick::interfaces::time::ITimeProvider&    time_;
    brick::interfaces::logging::ILogger&       logger_;
    PartitionAssets                            assets_;
    LvglBuffers                                buffers_;
    brick::platform::esp32::LvglDisplayAdapter display_adapter_;
    brick::platform::esp32::LvglTouchAdapter   touch_adapter_;
    lv_image_dsc_t                             image_descriptor_{};
    lv_obj_t*                                  image_    = nullptr;
    lv_obj_t*                                  status_   = nullptr;
    std::uint8_t                               selected_ = 0;
    std::uint32_t                              counter_  = 0;
};
}  // namespace cyd_lvgl_asset_smoke
