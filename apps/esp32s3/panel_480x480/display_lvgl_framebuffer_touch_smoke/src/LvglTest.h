#pragma once

#include "brick/core/time/Timer.h"
#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/display/IFrameBufferDisplay.h"
#include "brick/interfaces/display/ITouchscreen.h"
#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/time/ITimeProvider.h"
#include "brick/lvgl/LvglDisplayAdapter.h"
#include "brick/lvgl/LvglTouchAdapter.h"
#include "lvgl.h"

class LvglTest final
{
public:
    LvglTest(brick::interfaces::display::IDisplayDevice&      display,
             brick::interfaces::display::IFrameBufferDisplay& framebuffers,
             brick::interfaces::display::ITouchscreen& touch, brick::interfaces::logging::ILogger& logger,
             brick::interfaces::time::ITimeProvider& time);

    bool initialize();
    void update();

private:
    static void button_event_(lv_event_t* event);
    void        create_user_interface_();

    brick::interfaces::display::IDisplayDevice&      display_;
    brick::interfaces::display::IFrameBufferDisplay& framebuffers_;
    brick::interfaces::display::ITouchscreen&        touch_;
    brick::interfaces::logging::ILogger&             logger_;
    brick::lvgl::LvglDisplayAdapter       display_adapter_;
    brick::lvgl::LvglTouchAdapter         touch_adapter_;
    brick::core::time::Timer                         tick_timer_;
    brick::core::time::Timer                         frame_timer_;
    lv_obj_t*                                        title_         = nullptr;
    lv_obj_t*                                        press_label_   = nullptr;
    std::uint32_t                                    press_counter_ = 0;
    std::uint32_t                                    frame_counter_ = 0;
};
