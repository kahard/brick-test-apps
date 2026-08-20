#include <Arduino.h>

#include <array>
#include <cstdint>

#include "assets/joy_tears_240_rgb565.h"
#include "assets/sweat_smile_240_rgb565.h"
#include "brick/platform/esp8266/profiles/esp12f_st7789_240x240_ttp223.h"

namespace {
constexpr std::uint16_t kWidth = 240;
constexpr std::uint16_t kHeight = 240;
#ifndef BRICK_STREAM_STRIPE_HEIGHT
#define BRICK_STREAM_STRIPE_HEIGHT 20
#endif
constexpr std::uint16_t kStripeHeight = BRICK_STREAM_STRIPE_HEIGHT;
constexpr std::size_t kStripeBytes = static_cast<std::size_t>(kWidth) * kStripeHeight * 2;
static std::array<std::uint8_t, kStripeBytes> pixels{};

brick::platform::esp8266::St7789TftDisplay display(
    brick::platform::esp8266::profiles::esp12f_st7789_240x240());
bool stream_asset(const brick_image_asset_t& asset) {
  if (asset.width != kWidth || asset.height != kHeight || asset.bytes_per_pixel != 2 || asset.data == nullptr)
    return false;
  for (std::uint16_t y = 0; y < kHeight; y += kStripeHeight) {
    memcpy_P(pixels.data(), asset.data + static_cast<std::size_t>(y) * kWidth * 2, kStripeBytes);
    const brick::interfaces::display::PixelBuffer buffer{
        pixels.data(), kWidth, kStripeHeight, static_cast<std::size_t>(kWidth) * 2,
        brick::interfaces::display::PixelFormat::rgb565, false};
    if (!display.draw_buffer({0, y, kWidth, kStripeHeight}, buffer))
      return false;
  }
  return true;
}
}  // namespace

void setup() {
  Serial.begin(115200);
  delay(50);

  Serial.println();
  Serial.println("BRICK ESP-12F ST7789 asset streaming smoke test");
  if (!display.begin()) {
    Serial.println("Display initialization failed");
    return;
  }
  Serial.printf("Display initialized; stripe_bytes=%u asset_bytes=%u\n",
                static_cast<unsigned>(kStripeBytes),
                static_cast<unsigned>(brick_joy_tears_240.data_size));
}

void loop() {
  static std::uint32_t frame = 0;
  static std::uint32_t benchmark_start = millis();
  const auto& asset = (frame & 1U) ? brick_sweat_smile_240 : brick_joy_tears_240;
  const auto started = micros();
  if (!stream_asset(asset)) {
    Serial.println("Asset streaming failed");
    delay(1000);
    return;
  }
  ++frame;
  if (frame % 60 == 0) {
    const auto elapsed = millis() - benchmark_start;
    Serial.printf("stream benchmark: frames=60 elapsed=%lums fps=%.2f\n",
                  static_cast<unsigned long>(elapsed),
                  elapsed == 0 ? 0.0 : (60000.0 / elapsed));
    benchmark_start = millis();
  }
  if (frame % 10 == 0) {
    Serial.printf("streamed frame=%lu asset=%s elapsed=%luus\n",
                  static_cast<unsigned long>(frame),
                  ((frame - 1U) & 1U) ? "sweat_smile" : "joy_tears",
                  static_cast<unsigned long>(micros() - started));
  }
}
