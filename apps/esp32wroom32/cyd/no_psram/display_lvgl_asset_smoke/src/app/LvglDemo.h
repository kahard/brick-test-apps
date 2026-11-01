#pragma once

#include <cstdint>

#include "LvglBuffers.h"
#include "Services.h"
#include "brick/lvgl/LvglDisplayAdapter.h"
#include "brick/lvgl/LvglTouchAdapter.h"
#include "lvgl.h"

namespace cyd_lvgl_asset_smoke
{
/// One bundle image, a status line and a button that switches the image; the
/// image is read from the asset source into RAM on each switch.
class LvglDemo final
{
public:
    explicit LvglDemo(const Services& services);
    Status initialize();
    void   update();

private:
    static void on_button_clicked(lv_event_t* event);
    Status      load_selected_asset();
    void        create_widgets();

    TimeProvider&                   time_;
    Logger&                         logger_;
    AssetSource&                    assets_;
    LvglBuffers                     buffers_;
    brick::lvgl::LvglDisplayAdapter display_adapter_;
    brick::lvgl::LvglTouchAdapter   touch_adapter_;
    lv_image_dsc_t                  image_descriptor_{};
    lv_obj_t*                       image_         = nullptr;
    lv_obj_t*                       status_        = nullptr;
    std::uint8_t                    selected_      = 0;
    std::uint32_t                   counter_       = 0;
    std::uint32_t                   previous_tick_ = 0;
};
}  // namespace cyd_lvgl_asset_smoke
