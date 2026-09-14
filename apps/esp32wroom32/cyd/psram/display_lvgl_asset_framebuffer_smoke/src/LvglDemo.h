#pragma once

#include "LvglBuffers.h"
#include "PartitionAssets.h"
#include "Types.h"
#include "brick/platform/esp32/LvglDisplayAdapter.h"
#include "brick/platform/esp32/LvglTouchAdapter.h"
#include "lvgl.h"

namespace cyd_lvgl_asset_smoke
{
class LvglDemo final
{
public:
    explicit LvglDemo(Board& board);
    bool initialize();
    void update();

private:
    static void on_button_clicked(lv_event_t* event);
    bool        load_selected_asset();
    void        create_widgets();

    Board&                                     board_;
    PartitionAssets                            assets_;
    LvglBuffers                                buffers_;
    brick::platform::esp32::LvglDisplayAdapter display_adapter_{ board_.display() };
    brick::platform::esp32::LvglTouchAdapter   touch_adapter_{ board_.touch() };
    lv_image_dsc_t                             image_descriptor_{};
    lv_obj_t*                                  image_    = nullptr;
    lv_obj_t*                                  status_   = nullptr;
    std::uint8_t                               selected_ = 0;
    std::uint32_t                              counter_  = 0;
};
}  // namespace cyd_lvgl_asset_smoke
