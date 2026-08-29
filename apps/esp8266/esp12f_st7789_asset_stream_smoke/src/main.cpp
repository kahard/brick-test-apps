#include <Arduino.h>

#include <array>
#include <cstdint>

#include "asset_bundle.h"
#include "generated_assets.h"
#include "ProgmemAssetSource.h"
#include "brick/core/image/AssetStreamer.h"
#include "brick/platform/esp8266/profiles/esp12f_st7789_240x240_ttp223.h"

namespace {
constexpr std::uint16_t kWidth = 240;
constexpr std::uint16_t kHeight = 240;
constexpr std::uint16_t kStripeHeight = 20;
constexpr std::size_t kStripeBytes = static_cast<std::size_t>(kWidth) * kStripeHeight * 2;
static std::array<std::uint8_t, kStripeBytes> pixels{};

brick::platform::esp8266::St7789TftDisplay display(
    brick::platform::esp8266::profiles::esp12f_st7789_240x240());
brick::platform::esp8266::GpioButton button(
    brick::platform::esp8266::profiles::esp12f_ttp223_gpio4());
ProgmemAssetSource asset_source(brick_asset_bundle, generated_assets::bundle_size);
brick::core::image::AssetStreamer streamer(display);

bool show_asset(generated_assets::Id id, unsigned long& elapsed_us) {
  const auto* asset = generated_assets::get(id);
  if (asset == nullptr)
    return false;
  const auto started = micros();
  const bool displayed = streamer.stream(*asset, asset_source, {0, 0, kWidth, kHeight},
                                          pixels.data(), pixels.size());
  elapsed_us = micros() - started;
  return displayed;
}
}  // namespace

void setup() {
  Serial.begin(115200);
  delay(50);

  Serial.println();
  Serial.println("BRICK ESP-12F ST7789 bundled asset/button smoke test");
  if (!display.begin()) {
    Serial.println("Display initialization failed");
    return;
  }
  if (!button.begin()) {
    Serial.println("Button initialization failed");
    return;
  }
  unsigned long elapsed_us = 0;
  if (!show_asset(generated_assets::Id::joy_tears, elapsed_us)) {
    Serial.println("Initial bundled asset streaming failed");
    return;
  }
  Serial.printf("Display initialized; bundle_bytes=%u initial=joy_tears elapsed=%luus GPIO4 ready\n",
                static_cast<unsigned>(generated_assets::bundle_size), elapsed_us);
}

void loop() {
  static bool last_pressed = button.is_pressed();
  static generated_assets::Id selected = generated_assets::Id::joy_tears;
  const bool pressed = button.is_pressed();
  if (pressed && !last_pressed) {
    selected = selected == generated_assets::Id::joy_tears
                 ? generated_assets::Id::sweat_smile
                 : generated_assets::Id::joy_tears;
    unsigned long elapsed_us = 0;
    if (!show_asset(selected, elapsed_us)) {
      Serial.println("Bundled asset streaming failed");
    } else {
      Serial.printf("GPIO4 pressed; toggled asset=%s elapsed=%luus\n",
                    selected == generated_assets::Id::sweat_smile ? "sweat_smile" : "joy_tears",
                    elapsed_us);
    }
  }
  last_pressed = pressed;
  delay(20);
}
