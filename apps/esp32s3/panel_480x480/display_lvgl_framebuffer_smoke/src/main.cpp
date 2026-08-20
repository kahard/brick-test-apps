#include <cstdint>

#include "brick/interfaces/display/IFrameBufferDisplay.h"
#include "brick/platform/esp32/LvglDisplayAdapter.h"
#include "brick/platform/esp32/s3/profiles/st7701s_480x480.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lvgl.h"

namespace {
constexpr char TAG[] = "brick_st7701s_lvgl_fb";

auto panel_config() {
  auto config = brick::platform::esp32::s3::profiles::st7701s_480x480();
  config.frame_buffer_count = 2;
  return config;
}

brick::platform::esp32::s3::St7701sRgbDisplay display(panel_config());
}

extern "C" void app_main() {
  ESP_LOGI(TAG, "Starting ESP32-S3 ST7701S LVGL framebuffer smoke test");
  if (!display.begin()) {
    ESP_LOGE(TAG, "ST7701S RGB display initialization failed");
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

  auto* screen = lv_screen_active();
  lv_obj_set_style_bg_color(screen, lv_color_hex(0x102040), 0);
  auto* title = lv_label_create(screen);
  lv_label_set_text(title, "BRICK + LVGL DIRECT\nframe=0");
  lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
  lv_obj_center(title);

  ESP_LOGI(TAG, "LVGL initialized: mode=DIRECT framebuffers=%u",
           framebuffers.frame_buffer_count());

  std::uint32_t elapsed_ms = 0;
  std::uint32_t frame_counter = 0;
  while (true) {
    constexpr std::uint32_t kStepMs = 10;
    vTaskDelay(pdMS_TO_TICKS(kStepMs));
    elapsed_ms += kStepMs;
    lv_tick_inc(kStepMs);
    lv_timer_handler();

    if (elapsed_ms >= 1000) {
      elapsed_ms = 0;
      ++frame_counter;
      lv_label_set_text_fmt(title, "BRICK + LVGL DIRECT\nframe=%u",
                            static_cast<unsigned>(frame_counter));
      ESP_LOGI(TAG, "LVGL direct page flip active frame=%u",
               static_cast<unsigned>(frame_counter));
    }
  }
}
