#include <Arduino.h>

#include <array>
#include <cstdint>

#include "brick/platform/esp8266/profiles/esp12f_st7789_240x240_ttp223.h"

namespace {
constexpr std::uint16_t kWidth = 240;
constexpr std::uint16_t kStripeHeight = 40;
constexpr std::uint16_t kStatusHeight = 16;
constexpr std::uint32_t kPollIntervalMs = 20;
static std::array<std::uint8_t, kWidth * kStripeHeight * 2> pixels{};

brick::platform::esp8266::St7789TftDisplay display(
    brick::platform::esp8266::profiles::esp12f_st7789_240x240());
brick::platform::esp8266::GpioButton button(
    brick::platform::esp8266::profiles::esp12f_ttp223_gpio4());

void fill_rows(std::uint16_t y, std::uint16_t height, std::uint16_t color) {
  for (std::size_t index = 0; index < kWidth * height; ++index) {
    pixels[index * 2] = static_cast<std::uint8_t>(color >> 8);
    pixels[index * 2 + 1] = static_cast<std::uint8_t>(color);
  }
  display.draw_pixels(0, y, kWidth, height, pixels.data(),
                      static_cast<std::size_t>(kWidth) * height * 2);
}

void draw_test_pattern() {
  constexpr std::uint16_t colors[] = {0xF800, 0x07E0, 0x001F,
                                      0xFFE0, 0xF81F, 0x07FF};
  for (std::uint16_t index = 0; index < 6; ++index)
    fill_rows(index * kStripeHeight, kStripeHeight, colors[index]);
}

void draw_button_state(bool pressed) {
  fill_rows(kWidth - kStatusHeight, kStatusHeight,
            pressed ? 0x07E0 : 0x4208);
}
}  // namespace

void setup() {
  Serial.begin(115200);
  delay(50);

  Serial.println();
  Serial.println("BRICK ESP-12F ST7789/button smoke test");
  if (!display.begin()) {
    Serial.println("Display initialization failed");
    return;
  }
  if (!button.begin()) {
    Serial.println("Button initialization failed");
    return;
  }

  draw_test_pattern();
  const bool pressed = button.is_pressed();
  draw_button_state(pressed);
  Serial.printf("Display initialized; GPIO4 pressed=%u\n", pressed ? 1 : 0);
}

void loop() {
  static bool last_pressed = false;
  static bool initialized = false;
  if (!initialized) {
    last_pressed = button.is_pressed();
    initialized = true;
  }

  const bool pressed = button.is_pressed();
  if (pressed != last_pressed) {
    last_pressed = pressed;
    draw_button_state(pressed);
    Serial.printf("GPIO4 button %s\n", pressed ? "pressed" : "released");
  }
  delay(kPollIntervalMs);
}
