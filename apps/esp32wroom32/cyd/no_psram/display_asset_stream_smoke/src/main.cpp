#include <cstddef>
#include <cstdint>

#include "PartitionAssetSource.h"
#include "brick/core/image/AssetStreamer.h"
#include "brick/interfaces/display/PixelBuffer.h"
#include "brick/interfaces/display/TouchscreenTypes.h"
#include "brick/platform/esp32/Ili9341SpiDisplay.h"
#include "brick/platform/esp32/touch/Xpt2046Touchscreen.h"
#include "brick/platform/esp32/wroom32/profiles/cyd_ili9341_320x240.h"
#include "brick/platform/esp32/wroom32/profiles/cyd_xpt2046.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "generated_assets.h"

namespace
{
constexpr char TAG[] = "brick_cyd_stream";
constexpr std::uint16_t kWidth = 320;
constexpr std::uint16_t kHeight = 240;
constexpr std::uint16_t kStripeHeight = 16;
constexpr std::size_t kStripeBytes = static_cast<std::size_t>(kWidth) * kStripeHeight * 2;

brick::platform::esp32::Ili9341SpiDisplay display(
    brick::platform::esp32::profiles::cyd_ili9341_320x240());
brick::platform::esp32::touch::Xpt2046Touchscreen touch(
    brick::platform::esp32::profiles::cyd_xpt2046());
PartitionAssetSource asset_source("assets");
brick::core::image::AssetStreamer streamer(display);
std::uint8_t* stripe = nullptr;
generated_assets::Id selected = generated_assets::Id::joy_tears;

bool show(generated_assets::Id id)
{
    const auto* asset = generated_assets::find(id);
    if (asset == nullptr)
        return false;
    const auto started = esp_timer_get_time();
    if (!streamer.stream(*asset, asset_source, {0, 0, kWidth, kHeight}, stripe, kStripeBytes))
        return false;
    ESP_LOGI(TAG, "asset=%s streamed elapsed=%lldus free_internal=%u",
             id == generated_assets::Id::joy_tears ? "joy_tears" : "sweat_smile",
             static_cast<long long>(esp_timer_get_time() - started),
             static_cast<unsigned>(heap_caps_get_free_size(MALLOC_CAP_INTERNAL)));
    return true;
}
}  // namespace

extern "C" void app_main()
{
    ESP_LOGI(TAG, "Starting CYD internal-RAM stripe asset/touch test: %ux%u stripe_bytes=%u",
             kWidth, kHeight, static_cast<unsigned>(kStripeBytes));
    stripe = static_cast<std::uint8_t*>(heap_caps_malloc(kStripeBytes, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL));
    if (stripe == nullptr || !display.begin() || !touch.begin() || !asset_source.begin()) {
        ESP_LOGE(TAG, "initialization failed; this test requires internal DMA RAM and XPT2046 touch");
        return;
    }

    bool was_pressed = false;
    while (true) {
        if (!show(selected)) {
            ESP_LOGE(TAG, "asset stream failed");
            return;
        }

        const auto deadline = xTaskGetTickCount() + pdMS_TO_TICKS(1000);
        while (xTaskGetTickCount() < deadline) {
            brick::interfaces::display::TouchPoint point{};
            std::size_t count = 0;
            const bool pressed = touch.read(&point, 1, count) && count != 0 &&
                                 point.state != brick::interfaces::display::TouchState::released;
            if (pressed && !was_pressed) {
                selected = selected == generated_assets::Id::joy_tears
                               ? generated_assets::Id::sweat_smile
                               : generated_assets::Id::joy_tears;
                ESP_LOGI(TAG, "touch pressed at (%d,%d); next asset=%s",
                         point.x, point.y,
                         selected == generated_assets::Id::joy_tears ? "joy_tears" : "sweat_smile");
            }
            was_pressed = pressed;
            vTaskDelay(pdMS_TO_TICKS(20));
        }
    }
}
