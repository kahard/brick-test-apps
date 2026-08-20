#include <Arduino.h>

#include <array>
#include <cstdint>

#include "brick/core/image/AssetStreamer.h"
#include "brick/platform/esp8266/profiles/esp12f_st7789_240x240_ttp223.h"

namespace
{
constexpr std::uint16_t kWidth = 240;
constexpr std::uint16_t kHeight = 240;
constexpr std::uint16_t kStripeHeight = 40;
constexpr std::size_t kScratchBytes = static_cast<std::size_t>(kWidth) * kStripeHeight * 2;

class SolidColorReader final : public brick::interfaces::display::IAssetReader
{
public:
    bool read(const brick::interfaces::display::ImageAsset& asset, std::size_t offset,
              std::uint8_t* destination, std::size_t bytes) override
    {
        if (destination == nullptr || asset.data == nullptr || offset + bytes > asset.data_size)
            return false;

        std::uint8_t color[2] = {};
        memcpy_P(color, asset.data, sizeof(color));
        for (std::size_t i = 0; i < bytes; i += 2)
        {
            destination[i] = color[0];
            if (i + 1 < bytes)
                destination[i + 1] = color[1];
        }
        return true;
    }
};

brick::platform::esp8266::St7789TftDisplay display(
    brick::platform::esp8266::profiles::esp12f_st7789_240x240());
SolidColorReader reader;
brick::core::image::AssetStreamer streamer(display, reader);
std::array<std::uint8_t, kScratchBytes> scratch{};

const std::uint8_t kRedRgb565[] PROGMEM = {0x00, 0xF8};
const std::uint8_t kBlueRgb565[] PROGMEM = {0x1F, 0x00};

brick::interfaces::display::ImageAsset make_asset(const std::uint8_t* color)
{
    return {color, kWidth, kHeight, static_cast<std::size_t>(kWidth) * 2,
            static_cast<std::size_t>(kWidth) * kHeight * 2,
            brick::interfaces::display::PixelFormat::rgb565};
}
}  // namespace

void setup()
{
    Serial.begin(115200);
    delay(50);
    Serial.println("BRICK ESP-12F ST7789 AssetStreamer color boundary test");
    if (!display.begin())
    {
        Serial.println("Display initialization failed");
        return;
    }
    Serial.printf("Display initialized; stripe_bytes=%u asset_bytes=%u\n",
                  static_cast<unsigned>(kScratchBytes),
                  static_cast<unsigned>(kWidth * kHeight * 2));
}

void loop()
{
    static std::uint32_t frame = 0;
    static std::uint32_t benchmark_start = millis();
    const bool blue = (frame & 1U) != 0;
    const auto asset = make_asset(blue ? kBlueRgb565 : kRedRgb565);
    const auto started = micros();
    if (!streamer.stream(asset, {0, 0, kWidth, kHeight}, scratch.data(), scratch.size()))
    {
        Serial.println("AssetStreamer failed");
        delay(1000);
        return;
    }
    ++frame;
    if (frame % 60 == 0)
    {
        const auto elapsed = millis() - benchmark_start;
        Serial.printf("api benchmark: frames=60 elapsed=%lums fps=%.2f\n",
                      static_cast<unsigned long>(elapsed),
                      elapsed == 0 ? 0.0 : (60000.0 / elapsed));
        benchmark_start = millis();
    }
    if (frame % 10 == 0)
        Serial.printf("api streamed frame=%lu asset=%s elapsed=%luus\n",
                      static_cast<unsigned long>(frame), blue ? "blue" : "red",
                      static_cast<unsigned long>(micros() - started));
}
