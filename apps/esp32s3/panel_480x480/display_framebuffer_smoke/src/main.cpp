#include <cstdint>

#include "brick/interfaces/display/IFrameBufferDisplay.h"
#include "brick/boards/esp32/s3/Panel480Board.h"
#include "brick/core/time/Timer.h"
#include "brick/platform/esp32/s3/profiles/st7701s_480x480.h"
#include "esp_log.h"
#include "esp_timer.h"

namespace {
constexpr char TAG[] = "brick_st7701s_framebuffer";
constexpr std::uint16_t kWidth = 480;
constexpr std::uint16_t kHeight = 480;

auto panel_config() {
  auto config = brick::platform::esp32::s3::profiles::st7701s_480x480();
  config.frame_buffer_count = 2;
  return config;
}

brick::platform::esp32::s3::Panel480Board board(panel_config());
auto& display = board.display();

bool fill_and_present(brick::interfaces::display::IFrameBufferDisplay& framebuffers,
                      std::uint8_t index, std::uint16_t color) {
  brick::interfaces::display::WritablePixelBuffer buffer;
  if (!framebuffers.get_frame_buffer(index, buffer)) {
    ESP_LOGE(TAG, "Unable to acquire framebuffer %u", index);
    return false;
  }

  const auto start_us = esp_timer_get_time();
  auto* pixels = reinterpret_cast<std::uint16_t*>(buffer.data);
  for (std::size_t pixel = 0; pixel < kWidth * kHeight; ++pixel)
    pixels[pixel] = color;
  const auto fill_us = esp_timer_get_time() - start_us;

  if (!framebuffers.present_frame_buffer(index) || !display.wait_for_vsync(100)) {
    ESP_LOGE(TAG, "Unable to present framebuffer %u", index);
    return false;
  }
  const auto total_us = esp_timer_get_time() - start_us;
  ESP_LOGI(TAG, "framebuffer=%u color=0x%04X fill=%lldus present+vsync=%lldus",
           index, color, static_cast<long long>(fill_us),
           static_cast<long long>(total_us));
  return true;
}
}  // namespace

extern "C" void app_main() {
  ESP_LOGI(TAG, "Starting ESP32-S3 ST7701S framebuffer smoke test");
  if (!display.begin()) {
    ESP_LOGE(TAG, "ST7701S RGB display initialization failed");
    return;
  }

  auto& framebuffers = static_cast<brick::interfaces::display::IFrameBufferDisplay&>(display);
  ESP_LOGI(TAG, "Display initialized, framebuffer count=%u",
           framebuffers.frame_buffer_count());
  if (framebuffers.frame_buffer_count() < 2) {
    ESP_LOGE(TAG, "Two framebuffers are required for this test");
    return;
  }

  std::uint8_t index = 0;
  while (true) {
    const auto color = index == 0 ? 0xF800 : 0x001F;
    if (!fill_and_present(framebuffers, index, color))
      return;
    index ^= 1;
    board.time().delay_ms(500);
  }
}
