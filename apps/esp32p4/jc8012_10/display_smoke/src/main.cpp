#include <array>
#include <cstdint>

#include "brick/platform/esp32/p4/profiles/jd9365_800x1280.h"
#include "driver/gpio.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace {
constexpr char TAG[] = "brick_jc8012_10";

#ifndef BRICK_PANEL_ROTATION
#define BRICK_PANEL_ROTATION 0
#endif

#if BRICK_PANEL_ROTATION == 0
constexpr auto kRotation = brick::interfaces::display::Rotation::rotate_0;
constexpr std::uint16_t kWidth = 800;
constexpr std::uint16_t kStripeHeight = 160;
#elif BRICK_PANEL_ROTATION == 90
constexpr auto kRotation = brick::interfaces::display::Rotation::rotate_90;
constexpr std::uint16_t kWidth = 1280;
constexpr std::uint16_t kStripeHeight = 100;
#elif BRICK_PANEL_ROTATION == 180
constexpr auto kRotation = brick::interfaces::display::Rotation::rotate_180;
constexpr std::uint16_t kWidth = 800;
constexpr std::uint16_t kStripeHeight = 160;
#elif BRICK_PANEL_ROTATION == 270
constexpr auto kRotation = brick::interfaces::display::Rotation::rotate_270;
constexpr std::uint16_t kWidth = 1280;
constexpr std::uint16_t kStripeHeight = 100;
#else
#error "BRICK_PANEL_ROTATION must be 0, 90, 180 or 270"
#endif

constexpr std::uint16_t kLogicalHeight =
    (BRICK_PANEL_ROTATION == 90 || BRICK_PANEL_ROTATION == 270) ? 800 : 1280;
constexpr std::uint16_t kMarkerSize = 120;

brick::platform::esp32::p4::MipiDsiDisplay display(
    brick::platform::esp32::p4::profiles::jd9365_800x1280(
        kRotation));

void draw_test_pattern() {
#if BRICK_PANEL_ROTATION != 0
  const auto frame_pixels = static_cast<std::size_t>(kWidth) * kLogicalHeight;
  auto* frame = static_cast<std::uint16_t*>(
      heap_caps_calloc(frame_pixels, sizeof(std::uint16_t),
                       MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
  if (frame == nullptr) {
    ESP_LOGE(TAG, "unable to allocate logical frame (%zu bytes)",
             frame_pixels * sizeof(std::uint16_t));
    return;
  }
  constexpr std::uint16_t colors[] = {0xF800, 0x07E0, 0x001F, 0xFFE0,
                                      0xF81F, 0x07FF, 0x0000, 0xFFFF};
  const auto stripe_height = static_cast<std::uint16_t>(kLogicalHeight / 8);
  for (std::uint16_t y = 0; y < kLogicalHeight; ++y) {
    const auto color = colors[y / stripe_height];
    for (std::uint16_t x = 0; x < kWidth; ++x)
      frame[static_cast<std::size_t>(y) * kWidth + x] = color;
  }
  for (std::uint16_t y = 0; y < kMarkerSize; ++y)
    for (std::uint16_t x = 0; x < kMarkerSize; ++x)
      frame[static_cast<std::size_t>(kLogicalHeight - kMarkerSize + y) *
                kWidth + x] = 0x0000;
  const brick::interfaces::display::PixelBuffer buffer{
      reinterpret_cast<const std::uint8_t*>(frame), kWidth, kLogicalHeight,
      static_cast<std::size_t>(kWidth) * 2,
      brick::interfaces::display::PixelFormat::rgb565, false};
  const bool submitted = display.draw_buffer(
      {0, 0, kWidth, kLogicalHeight}, buffer);
  ESP_LOGI(TAG, "full logical frame submitted=%d", submitted ? 1 : 0);
  if (submitted) display.wait_for_transfer_complete(2000);
  heap_caps_free(frame);
#else
  static std::array<std::uint16_t, kWidth * kStripeHeight> stripe{};
  constexpr std::uint16_t colors[] = {0xF800, 0x07E0, 0x001F, 0xFFE0,
                                      0xF81F, 0x07FF, 0x0000, 0xFFFF};
  for (std::uint16_t index = 0; index < 8; ++index) {
    stripe.fill(colors[index]);
    const brick::interfaces::display::PixelBuffer buffer{
        reinterpret_cast<const std::uint8_t*>(stripe.data()), kWidth,
        kStripeHeight, static_cast<std::size_t>(kWidth) * 2,
        brick::interfaces::display::PixelFormat::rgb565, false};
    const bool submitted = display.draw_buffer(
        {0, static_cast<std::int16_t>(index * kStripeHeight), kWidth,
         kStripeHeight}, buffer);
    ESP_LOGI(TAG, "stripe=%u submitted=%d", index, submitted ? 1 : 0);
    if (submitted) display.wait_for_transfer_complete(1000);
  }

  // Orientation marker: this must remain in the logical bottom-left corner.
  static std::array<std::uint16_t, kMarkerSize * kMarkerSize> marker{};
  marker.fill(0x0000);
  const brick::interfaces::display::PixelBuffer marker_buffer{
      reinterpret_cast<const std::uint8_t*>(marker.data()), kMarkerSize,
      kMarkerSize, static_cast<std::size_t>(kMarkerSize) * 2,
      brick::interfaces::display::PixelFormat::rgb565, false};
  const bool marker_submitted = display.draw_buffer(
      {0, static_cast<std::int16_t>(kLogicalHeight - kMarkerSize),
       kMarkerSize, kMarkerSize},
      marker_buffer);
  ESP_LOGI(TAG, "orientation marker bottom_left submitted=%d",
           marker_submitted ? 1 : 0);
  if (marker_submitted) display.wait_for_transfer_complete(1000);
#endif
}
}  // namespace

extern "C" void app_main() {
  ESP_LOGI(TAG, "Starting JC8012 10-inch JD9365 smoke test: logical=%ux%u rotation=%d",
           kWidth, kLogicalHeight, BRICK_PANEL_ROTATION);
  gpio_set_direction(GPIO_NUM_23, GPIO_MODE_OUTPUT);
  gpio_set_level(GPIO_NUM_23, 0);
  if (!display.begin()) {
    ESP_LOGE(TAG, "MIPI-DSI display initialization failed");
    return;
  }
  gpio_set_level(GPIO_NUM_23, 1);
  ESP_LOGI(TAG, "Display initialized; drawing color stripes");
  draw_test_pattern();
  while (true) vTaskDelay(pdMS_TO_TICKS(1000));
}
