#include <array>
#include <cstdint>

#include "brick/boards/esp32/p4/Jc1060Board.h"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace {
constexpr char TAG[] = "brick_display_touch_smoke";
constexpr std::uint16_t kWidth = 1024;
constexpr std::uint16_t kHeight = 600;
constexpr std::uint16_t kStripeHeight = 100;
constexpr std::uint16_t kMarkerSize = 100;
constexpr std::uint16_t kMarkerArm = 20;
constexpr std::uint16_t kMarkerBorder = 4;

brick::platform::esp32::p4::Jc1060Board board;
auto& display = board.display();
auto& touch = board.touch();

void draw_test_pattern() {
  static std::array<std::uint16_t, kWidth * kStripeHeight> stripe{};
  constexpr std::uint16_t colors[] = {0xF800, 0x07E0, 0x001F,
                                      0xFFE0, 0xF81F, 0x07FF};
  for (std::uint16_t stripe_index = 0; stripe_index < 6; ++stripe_index) {
    stripe.fill(colors[stripe_index]);
    const brick::interfaces::display::PixelBuffer buffer{
        reinterpret_cast<const std::uint8_t*>(stripe.data()), kWidth, kStripeHeight,
        static_cast<std::size_t>(kWidth) * sizeof(std::uint16_t),
        brick::interfaces::display::PixelFormat::rgb565, false};
    const bool submitted = display.draw_buffer({0, stripe_index * kStripeHeight, kWidth, kStripeHeight}, buffer);
    ESP_LOGI(TAG, "stripe=%u submitted=%d", stripe_index, submitted);
    if (submitted)
      display.wait_for_transfer_complete(1000);
  }
}

bool draw_marker(const brick::interfaces::display::TouchPoint& point, bool active) {
  static std::array<std::uint16_t, kMarkerSize * kMarkerSize> marker{};
  constexpr std::uint16_t colors[] = {0xF800, 0x07E0, 0x001F,
                                      0xFFE0, 0xF81F, 0x07FF};
  const auto left = point.x < kMarkerSize / 2 ? 0 : point.x - kMarkerSize / 2;
  const auto top = point.y < kMarkerSize / 2 ? 0 : point.y - kMarkerSize / 2;
  const auto x0 = left + kMarkerSize > kWidth ? kWidth - kMarkerSize : left;
  const auto y0 = top + kMarkerSize > kHeight ? kHeight - kMarkerSize : top;
  constexpr auto half = kMarkerSize / 2;
  constexpr auto arm = kMarkerArm / 2;
  for (std::uint16_t y = 0; y < kMarkerSize; ++y) {
    for (std::uint16_t x = 0; x < kMarkerSize; ++x) {
      const bool inner = (x >= half - arm && x < half + arm) ||
                         (y >= half - arm && y < half + arm);
      const bool outer = (x >= half - arm - kMarkerBorder && x < half + arm + kMarkerBorder) ||
                         (y >= half - arm - kMarkerBorder && y < half + arm + kMarkerBorder);
      marker[static_cast<std::size_t>(y) * kMarkerSize + x] =
          active && outer ? (inner ? 0x0000 : 0xFFFF) : colors[(y0 + y) / kStripeHeight];
    }
  }
  const brick::interfaces::display::PixelBuffer buffer{
      reinterpret_cast<const std::uint8_t*>(marker.data()), kMarkerSize, kMarkerSize,
      static_cast<std::size_t>(kMarkerSize) * sizeof(std::uint16_t),
      brick::interfaces::display::PixelFormat::rgb565, false};
  return display.draw_buffer({x0, y0, kMarkerSize, kMarkerSize}, buffer) &&
         display.wait_for_transfer_complete(1000);
}
}  // namespace

extern "C" void app_main() {
  ESP_LOGI(TAG, "Starting JC1060 display/touch smoke test");
  if (!board.begin()) {
    ESP_LOGE(TAG, "JC1060 board initialization failed");
    return;
  }
  ESP_LOGI(TAG, "Display and touch ready; drawing test pattern in 2 seconds");
  vTaskDelay(pdMS_TO_TICKS(2000));
  draw_test_pattern();

  std::array<brick::interfaces::display::TouchPoint, 5> points{};
  brick::interfaces::display::TouchPoint marker_point{};
  bool marker_visible = false;
  while (true) {
    std::size_t count = 0;
    if (touch.read(points.data(), points.size(), count)) {
      for (std::size_t index = 0; index < count; ++index) {
        const auto& point = points[index];
        const bool active = point.state != brick::interfaces::display::TouchState::released;
        const bool moved = active && (!marker_visible || marker_point.x != point.x || marker_point.y != point.y);
        if (!active) {
          if (marker_visible) draw_marker(marker_point, false);
          marker_visible = false;
        } else if (moved) {
          if (marker_visible) draw_marker(marker_point, false);
          marker_point = point;
          marker_visible = true;
          draw_marker(marker_point, true);
        }
        ESP_LOGI(TAG, "touch id=%u x=%d y=%d raw=(%d,%d) pressure=%d state=%u",
                 point.id, point.x, point.y, point.raw_x, point.raw_y,
                 point.pressure, static_cast<unsigned>(point.state));
      }
    }
    vTaskDelay(pdMS_TO_TICKS(16));
  }
}
