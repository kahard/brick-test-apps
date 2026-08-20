#include <cstdint>

#include "brick/interfaces/display/IFrameBufferDisplay.h"
#include "brick/platform/esp32/LvglDisplayAdapter.h"
#include "brick/platform/esp32/s3/profiles/st7701s_480x480.h"
#include "brick/platform/esp32/s3/profiles/st7701s_gt911.h"
#include "images/sweat_smile_rgb565.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lvgl.h"

LV_FONT_DECLARE(brick_roboto_24);

namespace {
constexpr char TAG[] = "brick_st7701s_lvgl_asset";
lv_obj_t* press_label = nullptr;
std::uint32_t press_counter = 0;

void button_event_cb(lv_event_t* event) {
  if (lv_event_get_code(event) != LV_EVENT_CLICKED) {
    return;
  }

  ++press_counter;
  lv_label_set_text_fmt(press_label, "Naciśnięcia: %u",
                        static_cast<unsigned>(press_counter));
  ESP_LOGI(TAG, "LVGL button clicked count=%u",
           static_cast<unsigned>(press_counter));
}

auto panel_config() {
  auto config = brick::platform::esp32::s3::profiles::st7701s_480x480();
  config.pixel_clock_hz = 16'000'000;
  config.frame_buffer_count = 2;
  return config;
}

brick::platform::esp32::s3::St7701sRgbDisplay display(panel_config());
brick::platform::esp32::touch::Gt911Touchscreen touch(
    brick::platform::esp32::s3::profiles::st7701s_gt911());

const lv_image_dsc_t image_dsc = {
    .header = {.cf = LV_COLOR_FORMAT_RGB565, .w = 100, .h = 100, .stride = 200},
    .data_size = 20000,
    .data = brick_sweat_smile.data,
};

extern const uint8_t joy_tears_start[] asm("_binary_joy_tears_rgb565_bin_start");
extern const uint8_t joy_tears_end[] asm("_binary_joy_tears_rgb565_bin_end");

extern const uint8_t full_background_start[] asm("_binary_joy_tears_stream_rgb565_bin_start");
extern const uint8_t full_background_end[] asm("_binary_joy_tears_stream_rgb565_bin_end");

const lv_image_dsc_t binary_image_dsc = {
    .header = {.cf = LV_COLOR_FORMAT_RGB565, .w = 100, .h = 100, .stride = 200},
    .data_size = static_cast<uint32_t>(joy_tears_end - joy_tears_start),
    .data = joy_tears_start,
};

const lv_image_dsc_t full_background_dsc = {
    .header = {.cf = LV_COLOR_FORMAT_RGB565, .w = 480, .h = 480, .stride = 960},
    .data_size = static_cast<uint32_t>(full_background_end - full_background_start),
    .data = full_background_start,
};

void read_touch(lv_indev_t*, lv_indev_data_t* data) {
  brick::interfaces::display::TouchPoint point{};
  std::size_t count = 0;
  if (touch.read(&point, 1, count) && count > 0 &&
      point.state != brick::interfaces::display::TouchState::released) {
    data->point.x = point.x;
    data->point.y = point.y;
    data->state = LV_INDEV_STATE_PRESSED;
    return;
  }
  data->state = LV_INDEV_STATE_RELEASED;
}
}

extern "C" void app_main() {
  ESP_LOGI(TAG, "Starting ESP32-S3 ST7701S LVGL asset smoke test");
  if (!display.begin()) {
    ESP_LOGE(TAG, "ST7701S RGB display initialization failed");
    return;
  }
  if (!touch.begin()) {
    ESP_LOGE(TAG, "GT911 initialization failed");
    return;
  }

  lv_init();
  brick::platform::esp32::LvglDisplayAdapter adapter(display);
  auto& framebuffers = static_cast<brick::interfaces::display::IFrameBufferDisplay&>(display);
  auto* lv_display = adapter.create_framebuffer(framebuffers);
  if (lv_display == nullptr) {
    ESP_LOGE(TAG, "Unable to create LVGL direct framebuffer display");
    return;
  }
  auto* indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, read_touch);

  auto* screen = lv_screen_active();
  lv_obj_set_style_bg_color(screen, lv_color_hex(0x102040), 0);
  auto* background = lv_image_create(screen);
  lv_image_set_src(background, &full_background_dsc);
  lv_obj_center(background);
  lv_obj_move_background(background);

  auto* title = lv_label_create(screen);
  lv_label_set_text(title, "BRICK + LVGL ASSETS");
  lv_obj_set_style_text_font(title, &brick_roboto_24, 0);
  lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 40);

  auto* image = lv_image_create(screen);
  lv_image_set_src(image, &image_dsc);
  lv_obj_align(image, LV_ALIGN_TOP_MID, -80, 90);

  auto* image_label = lv_label_create(screen);
  lv_label_set_text(image_label, "C asset");
  lv_obj_set_style_text_font(image_label, &brick_roboto_24, 0);
  lv_obj_align(image_label, LV_ALIGN_TOP_MID, -80, 195);

  auto* binary_image = lv_image_create(screen);
  lv_image_set_src(binary_image, &binary_image_dsc);
  lv_obj_align(binary_image, LV_ALIGN_TOP_MID, 80, 90);

  auto* binary_label = lv_label_create(screen);
  lv_label_set_text(binary_label, "BIN asset");
  lv_obj_set_style_text_font(binary_label, &brick_roboto_24, 0);
  lv_obj_align(binary_label, LV_ALIGN_TOP_MID, 80, 195);

  auto* button = lv_button_create(screen);
  lv_obj_set_size(button, 240, 96);
  lv_obj_align(button, LV_ALIGN_CENTER, 0, 110);
  lv_obj_add_event_cb(button, button_event_cb, LV_EVENT_CLICKED, nullptr);

  auto* button_label = lv_label_create(button);
  lv_label_set_text(button_label, "DOTKNIJ");
  lv_obj_set_style_text_font(button_label, &brick_roboto_24, 0);
  lv_obj_center(button_label);

  press_label = lv_label_create(screen);
  lv_obj_set_style_text_font(press_label, &brick_roboto_24, 0);
  lv_label_set_text(press_label, "Naciśnięcia: 0");
  lv_obj_align(press_label, LV_ALIGN_BOTTOM_MID, 0, -40);

  ESP_LOGI(TAG, "LVGL initialized: mode=DIRECT framebuffers=%u pclk=16MHz touch=GT911 background=BIN 480x480",
           framebuffers.frame_buffer_count());

  while (true) {
    constexpr std::uint32_t kStepMs = 10;
    vTaskDelay(pdMS_TO_TICKS(kStepMs));
    lv_tick_inc(kStepMs);
    lv_timer_handler();
  }
}
