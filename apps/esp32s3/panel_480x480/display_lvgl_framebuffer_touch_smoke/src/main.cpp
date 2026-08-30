#include <cstdint>

#include "brick/boards/esp32/s3/Panel480Board.h"
#include "brick/core/time/Timer.h"
#include "brick/interfaces/display/IFrameBufferDisplay.h"
#include "brick/platform/esp32/LvglDisplayAdapter.h"
#include "brick/platform/esp32/LvglTouchAdapter.h"
#include "brick/platform/esp32/s3/profiles/st7701s_480x480.h"
#include "brick/platform/esp32/s3/profiles/st7701s_gt911.h"
#include "lvgl.h"

LV_FONT_DECLARE(brick_font_24);

namespace
{
constexpr char                       TAG[]         = "brick_st7701s_lvgl_fb";
brick::interfaces::logging::ILogger* g_logger      = nullptr;
lv_obj_t*                            press_label   = nullptr;
std::uint32_t                        press_counter = 0;

void button_event_cb(lv_event_t* event)
{
    if (lv_event_get_code(event) != LV_EVENT_CLICKED)
    {
        return;
    }

    ++press_counter;
    lv_label_set_text_fmt(press_label, "Kliknięcia: %u", static_cast<unsigned>(press_counter));
    g_logger->info(TAG, "LVGL button clicked count=%u", static_cast<unsigned>(press_counter));
}

auto panel_config()
{
    auto config               = brick::platform::esp32::s3::profiles::st7701s_480x480();
    config.pixel_clock_hz     = 16'000'000;
    config.frame_buffer_count = 2;
    return config;
}

brick::platform::esp32::s3::Panel480Board board(panel_config());
auto&                                     display = board.display();
auto&                                     touch   = board.touch();

}  // namespace

extern "C" void app_main()
{
    g_logger = &board.logger();
    g_logger->info(TAG, "Starting ESP32-S3 ST7701S LVGL framebuffer + GT911 smoke test");
    if (!display.begin())
    {
        g_logger->error(TAG, "ST7701S RGB display initialization failed");
        return;
    }
    if (!touch.begin())
    {
        g_logger->error(TAG, "GT911 initialization failed");
        return;
    }

    lv_init();
    brick::platform::esp32::LvglDisplayAdapter adapter(display);
    auto& framebuffers = static_cast<brick::interfaces::display::IFrameBufferDisplay&>(display);
    auto* lv_display   = adapter.create_framebuffer(framebuffers);
    if (lv_display == nullptr)
    {
        g_logger->error(TAG, "Unable to create LVGL direct framebuffer display");
        return;
    }
    brick::platform::esp32::LvglTouchAdapter touch_adapter(touch);
    if (touch_adapter.create() == nullptr)
    {
        g_logger->error(TAG, "Unable to create LVGL touch input");
        return;
    }

    auto* screen = lv_screen_active();
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x102040), 0);
    auto* title = lv_label_create(screen);
    lv_label_set_text(title, "BRICK + LVGL DIRECT\nframe=0");
    lv_obj_set_style_text_font(title, &brick_font_24, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 40);

    auto* button = lv_button_create(screen);
    lv_obj_set_size(button, 240, 96);
    lv_obj_center(button);
    lv_obj_add_event_cb(button, button_event_cb, LV_EVENT_CLICKED, nullptr);

    auto* button_label = lv_label_create(button);
    lv_label_set_text(button_label, "DOTKNIJ");
    lv_obj_set_style_text_font(button_label, &brick_font_24, 0);
    lv_obj_center(button_label);

    press_label = lv_label_create(screen);
    lv_obj_set_style_text_font(press_label, &brick_font_24, 0);
    lv_label_set_text(press_label, "Kliknięcia: 0");
    lv_obj_align(press_label, LV_ALIGN_BOTTOM_MID, 0, -40);

    g_logger->info(TAG, "LVGL initialized: mode=DIRECT framebuffers=%u pclk=16MHz touch=GT911",
                   framebuffers.frame_buffer_count());

    std::uint32_t            frame_counter = 0;
    brick::core::time::Timer step_timer(board.time());
    brick::core::time::Timer frame_timer(board.time());
    step_timer.start(10);
    frame_timer.start(1000);
    while (true)
    {
        if (step_timer.expired())
        {
            step_timer.restart();
            lv_tick_inc(10);
            lv_timer_handler();
        }
        if (frame_timer.expired())
        {
            frame_timer.restart();
            ++frame_counter;
            lv_label_set_text_fmt(title, "BRICK + LVGL DIRECT\nframe=%u", static_cast<unsigned>(frame_counter));
            g_logger->info(TAG, "LVGL direct page flip active frame=%u", static_cast<unsigned>(frame_counter));
        }
        board.time().delay_ms(1);
    }
}
