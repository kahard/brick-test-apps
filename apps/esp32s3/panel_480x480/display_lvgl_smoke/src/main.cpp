#include <cstdint>

#include "brick/platform/esp32/LvglDisplayAdapter.h"
#include "brick/platform/esp32/s3/profiles/st7701s_480x480.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lvgl.h"

namespace {
constexpr char TAG[] = "brick_st7701s_lvgl";
constexpr std::uint32_t kBufferLines = 40;
constexpr std::size_t kBufferBytes = 480U * kBufferLines * sizeof(std::uint16_t);

brick::platform::esp32::s3::St7701sRgbDisplay display(
    brick::platform::esp32::s3::profiles::st7701s_480x480());

void* allocate_render_buffer() {
  auto* buffer = heap_caps_malloc(kBufferBytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  if (buffer == nullptr)
    buffer = heap_caps_malloc(kBufferBytes, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  return buffer;
}
}  // namespace

extern "C" void app_main() {
  ESP_LOGI(TAG, "Starting ESP32-S3 ST7701S LVGL smoke test");

  if (!display.begin()) {
    ESP_LOGE(TAG, "ST7701S RGB display initialization failed");
    return;
  }

  auto* buffer_1 = allocate_render_buffer();
  auto* buffer_2 = allocate_render_buffer();
  if (buffer_1 == nullptr || buffer_2 == nullptr) {
    if (buffer_1 != nullptr)
      heap_caps_free(buffer_1);
    if (buffer_2 != nullptr)
      heap_caps_free(buffer_2);
    ESP_LOGE(TAG, "Unable to allocate LVGL render buffers (%u bytes each)",
             static_cast<unsigned>(kBufferBytes));
    return;
  }

  lv_init();
  brick::platform::esp32::LvglDisplayAdapter adapter(display);
  auto* lv_display = adapter.create(LV_DISPLAY_RENDER_MODE_PARTIAL, buffer_1,
                                     buffer_2, kBufferBytes);
  if (lv_display == nullptr) {
    ESP_LOGE(TAG, "Unable to create LVGL display");
    return;
  }

  auto* screen = lv_screen_active();
  lv_obj_set_style_bg_color(screen, lv_color_hex(0x102040), 0);

  auto* title = lv_label_create(screen);
  lv_label_set_text(title, "BRICK + LVGL");
  lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
  lv_obj_center(title);

  ESP_LOGI(TAG, "LVGL initialized: mode=PARTIAL buffers=2 buffer_bytes=%u",
           static_cast<unsigned>(kBufferBytes));

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
      lv_label_set_text_fmt(title, "BRICK + LVGL\nframe=%u",
                            static_cast<unsigned>(frame_counter));
      ESP_LOGI(TAG, "LVGL render loop active");
    }
  }
}
