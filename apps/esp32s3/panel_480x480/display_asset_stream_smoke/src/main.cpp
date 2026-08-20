#include <cstdint>
#include <cstring>

#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/display/PixelBuffer.h"
#include "brick/platform/esp32/s3/profiles/st7701s_480x480.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace {
constexpr char TAG[] = "brick_st7701s_asset_stream";
constexpr std::uint16_t kWidth = 480;
constexpr std::uint16_t kHeight = 480;
constexpr std::uint16_t kStripeHeight = 20;
constexpr std::size_t kBytesPerPixel = 2;
constexpr std::size_t kStripeBytes = kWidth * kStripeHeight * kBytesPerPixel;

extern const std::uint8_t joy_tears_start[] asm("_binary_joy_tears_stream_rgb565_bin_start");
extern const std::uint8_t joy_tears_end[] asm("_binary_joy_tears_stream_rgb565_bin_end");
extern const std::uint8_t sweat_smile_start[] asm("_binary_sweat_smile_stream_rgb565_bin_start");
extern const std::uint8_t sweat_smile_end[] asm("_binary_sweat_smile_stream_rgb565_bin_end");

brick::platform::esp32::s3::St7701sRgbDisplay display(
    brick::platform::esp32::s3::profiles::st7701s_480x480());

bool stream_frame(const std::uint8_t* image_start,
                  const std::uint8_t* image_end, std::uint8_t* stripe) {
  const auto image_size = static_cast<std::size_t>(image_end - image_start);
  if (image_size != kWidth * kHeight * kBytesPerPixel) {
    ESP_LOGE(TAG, "Unexpected image size: %u", static_cast<unsigned>(image_size));
    return false;
  }

  for (std::uint16_t y = 0; y < kHeight; y += kStripeHeight) {
    const auto offset = static_cast<std::size_t>(y) * kWidth * kBytesPerPixel;
    std::memcpy(stripe, image_start + offset, kStripeBytes);
    const brick::interfaces::display::PixelBuffer buffer{
        stripe, kWidth, kStripeHeight, kWidth * kBytesPerPixel,
        brick::interfaces::display::PixelFormat::rgb565, false};
    if (!display.draw_buffer({0, y, kWidth, kStripeHeight}, buffer) ||
        !display.wait_for_transfer_complete(100)) {
      ESP_LOGE(TAG, "Stripe transfer failed at y=%u", y);
      return false;
    }
  }
  return true;
}
}  // namespace

extern "C" void app_main() {
  ESP_LOGI(TAG, "Starting ST7701S BIN streaming test: %ux%u stripe=%u rows",
           kWidth, kHeight, kStripeHeight);
  if (!display.begin()) {
    ESP_LOGE(TAG, "ST7701S RGB display initialization failed");
    return;
  }

  auto* stripe = static_cast<std::uint8_t*>(
      heap_caps_malloc(kStripeBytes, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL));
  if (stripe == nullptr) {
    ESP_LOGE(TAG, "Unable to allocate %u-byte stripe buffer",
             static_cast<unsigned>(kStripeBytes));
    return;
  }

  std::uint32_t frame = 0;
  std::uint32_t successful_frames = 0;
  std::int64_t window_started = esp_timer_get_time();
  while (true) {
    const auto started = esp_timer_get_time();
    const bool use_sweat_smile = (frame & 1U) != 0U;
    const auto* image_start = use_sweat_smile ? sweat_smile_start : joy_tears_start;
    const auto* image_end = use_sweat_smile ? sweat_smile_end : joy_tears_end;
    if (stream_frame(image_start, image_end, stripe)) {
      const auto elapsed = esp_timer_get_time() - started;
      ++frame;
      ++successful_frames;
      ESP_LOGI(TAG, "streamed frame=%u asset=%s bytes=%u elapsed=%lldus",
               static_cast<unsigned>(frame), use_sweat_smile ? "sweat_smile" : "joy_tears",
               static_cast<unsigned>(image_end - image_start), elapsed);
      if (successful_frames == 60) {
        const auto window_elapsed = esp_timer_get_time() - window_started;
        ESP_LOGI(TAG, "stream benchmark: frames=%u elapsed=%lldus fps=%.2f",
                 static_cast<unsigned>(successful_frames), window_elapsed,
                 static_cast<double>(successful_frames) * 1000000.0 /
                     static_cast<double>(window_elapsed));
        successful_frames = 0;
        window_started = esp_timer_get_time();
      }
    }
  }
}
