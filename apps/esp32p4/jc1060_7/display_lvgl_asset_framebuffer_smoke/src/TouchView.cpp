#include "TouchView.h"

TouchView::TouchView(LvglAssets& assets, brick::interfaces::logging::ILogger& logger) : assets_(assets), logger_(logger)
{
}

void TouchView::create()
{
    lv_obj_t* screen = lv_screen_active();
    lv_obj_set_style_text_font(screen, &lv_font_montserrat_24, 0);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x202040), 0);
    lv_obj_set_style_text_color(screen, lv_color_hex(0xFFFFFF), 0);
    lv_obj_t* title = lv_label_create(screen);
    lv_label_set_text(title, "JC1060 - LVGL FULL + TOUCH + ASSET BUNDLE");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 32);
    image_ = lv_image_create(screen);
    lv_image_set_src(image_, assets_.image(false));
    lv_obj_align(image_, LV_ALIGN_CENTER, 0, -40);
    lv_obj_t* button = lv_button_create(screen);
    lv_obj_set_size(button, 280, 80);
    lv_obj_align(button, LV_ALIGN_BOTTOM_MID, 0, -30);
    lv_obj_add_event_cb(button, clicked, LV_EVENT_CLICKED, this);
    lv_obj_t* label = lv_label_create(button);
    lv_label_set_text(label, "DOTKNIJ");
    lv_obj_center(label);
    status_ = lv_label_create(screen);
    lv_label_set_text(status_, "Klikniecia: 0");
    lv_obj_align(status_, LV_ALIGN_BOTTOM_MID, 0, -130);
}

void TouchView::clicked(lv_event_t* event)
{
    TouchView* self = static_cast<TouchView*>(lv_event_get_user_data(event));
    if (self != nullptr)
        self->change_image();
}

void TouchView::change_image()
{
    ++clicks_;
    lv_image_set_src(image_, assets_.image((clicks_ & 1U) != 0));
    lv_label_set_text_fmt(status_, "Klikniecia: %u", static_cast<unsigned>(clicks_));
    logger_.info("lvgl", "Button clicked count=%u", static_cast<unsigned>(clicks_));
}
