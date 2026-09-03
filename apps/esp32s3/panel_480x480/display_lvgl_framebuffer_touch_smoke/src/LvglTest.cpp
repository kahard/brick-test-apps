#include "LvglTest.h"

LV_FONT_DECLARE(brick_font_24);

namespace
{
constexpr char kTag[] = "brick_st7701s_lvgl_fb";
}

LvglTest::LvglTest(brick::interfaces::display::IDisplayDevice&      display,
                   brick::interfaces::display::IFrameBufferDisplay& framebuffers,
                   brick::interfaces::display::ITouchscreen& touch, brick::interfaces::logging::ILogger& logger,
                   brick::interfaces::time::ITimeProvider& time)
    : display_(display), framebuffers_(framebuffers), touch_(touch), logger_(logger), display_adapter_(display),
      touch_adapter_(touch), tick_timer_(time), frame_timer_(time)
{
}

bool LvglTest::initialize()
{
    logger_.info(kTag, "Starting ESP32-S3 ST7701S LVGL framebuffer + GT911 smoke test");
    if (!display_.begin() || !touch_.begin())
        return false;

    lv_init();
    if (display_adapter_.create_framebuffer(framebuffers_) == nullptr || touch_adapter_.create() == nullptr)
        return false;

    create_user_interface_();
    tick_timer_.start(10);
    frame_timer_.start(1000);
    logger_.info(kTag, "LVGL initialized: mode=DIRECT framebuffers=%u pclk=12MHz touch=GT911",
                 framebuffers_.frame_buffer_count());
    return true;
}

void LvglTest::update()
{
    if (tick_timer_.expired())
    {
        tick_timer_.restart();
        lv_tick_inc(10);
        lv_timer_handler();
    }
    if (frame_timer_.expired())
    {
        frame_timer_.restart();
        ++frame_counter_;
        lv_label_set_text_fmt(title_, "BRICK + LVGL DIRECT\nframe=%u", static_cast<unsigned>(frame_counter_));
        logger_.info(kTag, "LVGL direct page flip active frame=%u", static_cast<unsigned>(frame_counter_));
    }
}

void LvglTest::button_event_(lv_event_t* event)
{
    if (lv_event_get_code(event) != LV_EVENT_CLICKED)
        return;
    LvglTest* self = static_cast<LvglTest*>(lv_event_get_user_data(event));
    ++self->press_counter_;
    lv_label_set_text_fmt(self->press_label_, "Kliknięcia: %u", static_cast<unsigned>(self->press_counter_));
    self->logger_.info(kTag, "LVGL button clicked count=%u", static_cast<unsigned>(self->press_counter_));
}

void LvglTest::create_user_interface_()
{
    lv_obj_t* screen = lv_screen_active();
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x102040), 0);

    title_ = lv_label_create(screen);
    lv_label_set_text(title_, "BRICK + LVGL DIRECT\nframe=0");
    lv_obj_set_style_text_font(title_, &brick_font_24, 0);
    lv_obj_set_style_text_color(title_, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(title_, LV_ALIGN_TOP_MID, 0, 40);

    lv_obj_t* button = lv_button_create(screen);
    lv_obj_set_size(button, 240, 96);
    lv_obj_center(button);
    lv_obj_add_event_cb(button, button_event_, LV_EVENT_CLICKED, this);

    lv_obj_t* button_label = lv_label_create(button);
    lv_label_set_text(button_label, "DOTKNIJ");
    lv_obj_set_style_text_font(button_label, &brick_font_24, 0);
    lv_obj_center(button_label);

    press_label_ = lv_label_create(screen);
    lv_obj_set_style_text_font(press_label_, &brick_font_24, 0);
    lv_label_set_text(press_label_, "Kliknięcia: 0");
    lv_obj_align(press_label_, LV_ALIGN_BOTTOM_MID, 0, -40);
}
