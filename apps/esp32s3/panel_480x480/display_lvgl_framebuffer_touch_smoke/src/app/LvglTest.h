#pragma once

#include <cstdint>

#include "Services.h"
#include "brick/core/time/Timer.h"
#include "brick/lvgl/LvglDisplayAdapter.h"
#include "brick/lvgl/LvglTouchAdapter.h"
#include "lvgl.h"

namespace lvgl_framebuffer_touch_smoke
{
/// LVGL in DIRECT mode on the panel's two scan-out buffers with touch input:
/// a button counts clicks and the title shows a frame counter once a second.
class LvglTest final
{
public:
    explicit LvglTest(const Services& services);

    Status initialize();
    void   update();

private:
    static void button_event_(lv_event_t* event);
    void        create_user_interface_();

    FrameBufferDisplay&             framebuffers_;
    Logger&                         logger_;
    brick::lvgl::LvglDisplayAdapter display_adapter_;
    brick::lvgl::LvglTouchAdapter   touch_adapter_;
    brick::core::time::Timer        tick_timer_;
    brick::core::time::Timer        frame_timer_;
    lv_obj_t*                       title_         = nullptr;
    lv_obj_t*                       press_label_   = nullptr;
    std::uint32_t                   press_counter_ = 0;
    std::uint32_t                   frame_counter_ = 0;
};
}  // namespace lvgl_framebuffer_touch_smoke
