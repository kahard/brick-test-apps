#include <array>
#include <cstdint>

#include "brick/platform/esp32/wroom32/profiles/cyd_ili9341_320x240.h"
#include "brick/platform/esp32/wroom32/profiles/cyd_xpt2046.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace {
constexpr char TAG[] = "brick_cyd_smoke";
constexpr std::uint16_t kWidth = 320;
constexpr std::uint16_t kStripeHeight = 20;

brick::platform::esp32::Ili9341SpiDisplay display(
    brick::platform::esp32::profiles::cyd_ili9341_320x240());
brick::platform::esp32::touch::Xpt2046Touchscreen touch(
    brick::platform::esp32::profiles::cyd_xpt2046());

void draw_test_pattern() {
  static std::array<std::uint8_t, kWidth * kStripeHeight * 2> stripe{};
  constexpr std::uint16_t colors[] = {0xF800, 0x07E0, 0x001F, 0xFFE0,
                                      0xF81F, 0x07FF, 0xFFFF, 0x0000,
                                      0x7BEF, 0x4208, 0xFD20, 0xAFE5};
  for (std::uint16_t index = 0; index < 12; ++index) {
    const auto color = colors[index];
    for (std::size_t pixel = 0; pixel < kWidth * kStripeHeight; ++pixel) {
      stripe[pixel * 2] = static_cast<std::uint8_t>(color >> 8);
      stripe[pixel * 2 + 1] = static_cast<std::uint8_t>(color);
    }
    const brick::interfaces::display::PixelBuffer buffer{
        reinterpret_cast<const std::uint8_t*>(stripe.data()), kWidth, kStripeHeight,
        static_cast<std::size_t>(kWidth) * sizeof(std::uint16_t),
        brick::interfaces::display::PixelFormat::rgb565, false};
    if (!display.draw_buffer({0, index * kStripeHeight, kWidth, kStripeHeight}, buffer)) {
      ESP_LOGE(TAG, "draw failed for stripe %u", index);
    }
  }
}
}  // namespace

extern "C" void app_main() {
  ESP_LOGI(TAG, "Starting CYD ILI9341/XPT2046 smoke test");
  if (!display.begin()) {
    ESP_LOGE(TAG, "ILI9341 display initialization failed");
    return;
  }
  if (!touch.begin()) {
    ESP_LOGE(TAG, "XPT2046 touch initialization failed");
    return;
  }
  draw_test_pattern();
  ESP_LOGI(TAG, "Display and touch initialized");

  std::array<brick::interfaces::display::TouchPoint, 1> points{};
  while (true) {
    std::size_t count = 0;
    if (touch.read(points.data(), points.size(), count) && count != 0) {
      const auto& point = points[0];
      ESP_LOGI(TAG, "touch x=%d y=%d raw=(%d,%d) pressure=%d state=%u",
               point.x, point.y, point.raw_x, point.raw_y, point.pressure,
               static_cast<unsigned>(point.state));
    }
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}
