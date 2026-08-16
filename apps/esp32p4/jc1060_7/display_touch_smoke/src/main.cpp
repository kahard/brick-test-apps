#include <array>
#include <cstdint>

#include "brick/platform/esp32/p4/jc1060_1024x600.hpp"
#include "brick/platform/esp32/p4/jc1060_gt911.hpp"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace {
constexpr char TAG[] = "brick_display_touch_smoke";

// Temporary panel command sequence. The final test app should keep this
// sequence in a board-specific source file once the panel profile is stable.
constexpr std::uint8_t kInitSequence[] = {
    0x30, 0x00, 0xF7, 0x04, 0x49, 0x61, 0x02, 0x00,
    0x30, 0x01, 0x04, 0x0C, 0x05, 0x00, 0x06, 0x00,
    0x0B, 0x11, 0x17, 0x00, 0x20, 0x04, 0x1F, 0x05,
    0x23, 0x00, 0x25, 0x19, 0x28, 0x18, 0x29, 0x04,
    0x2A, 0x01, 0x2B, 0x04, 0x2C, 0x01, 0x30, 0x02,
    0x01, 0x22, 0x03, 0x12, 0x04, 0x00, 0x05, 0x64,
    0x0A, 0x08, 0x18, 0x99, 0x30, 0x0A, 0x02, 0x4F,
    0x0B, 0x40, 0x12, 0x3E, 0x13, 0x78, 0x30, 0x0D,
    0x0D, 0x04, 0x10, 0x0C, 0x11, 0x0C, 0x12, 0x0C,
    0x13, 0x0C, 0x30, 0x00, 0x3A, 0x01, 0x55, 0x11,
    0x00, 0x78, 0xFF, 0x29, 0x00, 0x14, 0xFF,
};

brick::platform::esp32::p4::MipiDsiDisplay display(
    brick::platform::esp32::p4::profiles::jc1060_1024x600(
        kInitSequence, sizeof(kInitSequence)));
brick::platform::esp32::touch::Gt911Touchscreen touch(
    brick::platform::esp32::p4::profiles::jc1060_gt911());

void draw_test_pattern() {
  constexpr std::uint16_t width = 1024;
  constexpr std::uint16_t height = 600;
  constexpr std::uint16_t stripe_height = 100;
  static std::array<std::uint16_t, width * stripe_height> stripe{};
  constexpr std::uint16_t colors[] = {0xF800, 0x07E0, 0x001F,
                                      0xFFE0, 0xF81F, 0x07FF};
  for (std::uint16_t stripe_index = 0; stripe_index < 6; ++stripe_index) {
    stripe.fill(colors[stripe_index]);
    display.draw_pixels(0, stripe_index * stripe_height, width, stripe_height,
                        reinterpret_cast<const std::uint8_t*>(stripe.data()),
                        stripe.size() * sizeof(std::uint16_t));
  }
}
}  // namespace

extern "C" void app_main() {
  ESP_LOGI(TAG, "Starting JC1060 display/touch smoke test");
  if (!display.begin()) {
    ESP_LOGE(TAG, "MIPI-DSI display initialization failed");
    return;
  }
  if (!touch.begin()) {
    ESP_LOGE(TAG, "GT911 initialization failed");
    return;
  }
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
