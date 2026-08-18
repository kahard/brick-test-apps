#include <array>
#include <cstdint>

#include "brick/platform/esp32/p4/profiles/guition_jc1060p470c_i_w.h"
#include "brick/platform/esp32/p4/profiles/jc1060_gt911.h"

#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace {
constexpr char TAG[] = "brick_display_touch_smoke";

brick::platform::esp32::p4::MipiDsiDisplay display(
    brick::platform::esp32::p4::profiles::guition_jc1060p470c_i_w());
brick::platform::esp32::touch::Gt911Touchscreen touch(
    brick::platform::esp32::p4::profiles::jc1060_gt911());

void draw_test_pattern() {
  constexpr std::uint16_t width = 1024;
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
  gpio_set_direction(GPIO_NUM_23, GPIO_MODE_OUTPUT);
  gpio_set_level(GPIO_NUM_23, 0);
  if (!display.begin()) {
    ESP_LOGE(TAG, "MIPI-DSI display initialization failed");
    return;
  }
  gpio_set_level(GPIO_NUM_23, 1);
  ESP_LOGI(TAG, "Display initialized and backlight enabled");
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
