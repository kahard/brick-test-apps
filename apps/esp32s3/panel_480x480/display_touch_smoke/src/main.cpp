#include <array>
#include <cstdint>

#include "brick/platform/esp32/s3/profiles/st7701s_480x480.hpp"
#include "brick/platform/esp32/s3/profiles/st7701s_gt911.hpp"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace {
constexpr char TAG[] = "brick_st7701s_smoke";
constexpr std::uint16_t kWidth = 480;
constexpr std::uint16_t kStripeHeight = 80;

brick::platform::esp32::s3::St7701sRgbDisplay display(
    brick::platform::esp32::s3::profiles::st7701s_480x480());
brick::platform::esp32::touch::Gt911Touchscreen touch(
    brick::platform::esp32::s3::profiles::st7701s_gt911());

void draw_test_pattern() {
  static std::array<std::uint16_t, kWidth * kStripeHeight> stripe{};
  constexpr std::uint16_t colors[] = {0xF800, 0x07E0, 0x001F,
                                      0xFFE0, 0xF81F, 0x07FF};
  for (std::uint16_t index = 0; index < 6; ++index) {
    stripe.fill(colors[index]);
    display.draw_pixels(0, index * kStripeHeight, kWidth, kStripeHeight,
                        reinterpret_cast<const std::uint8_t*>(stripe.data()),
                        stripe.size() * sizeof(std::uint16_t));
  }
}
}  // namespace

extern "C" void app_main() {
  ESP_LOGI(TAG, "Starting ESP32-S3 ST7701S/GT911 smoke test");
  if (!display.begin()) {
    ESP_LOGE(TAG, "ST7701S RGB display initialization failed");
    return;
  }
  ESP_LOGI(TAG, "Display initialized");
  if (!touch.begin()) {
    ESP_LOGE(TAG, "GT911 initialization failed");
    return;
  }
  ESP_LOGI(TAG, "Touch initialized");
  draw_test_pattern();

  std::array<brick::interfaces::display::TouchPoint, 5> points{};
  while (true) {
    std::size_t count = 0;
    if (touch.read(points.data(), points.size(), count)) {
      for (std::size_t index = 0; index < count; ++index) {
        const auto& point = points[index];
        ESP_LOGI(TAG, "touch id=%u x=%d y=%d raw=(%d,%d) pressure=%d state=%u",
                 point.id, point.x, point.y, point.raw_x, point.raw_y,
                 point.pressure, static_cast<unsigned>(point.state));
      }
    }
    vTaskDelay(pdMS_TO_TICKS(16));
  }
}
