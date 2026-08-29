#include <cstddef>
#include <cstdint>
#include <cstring>

#include "PartitionAssetSource.h"
#include "brick/core/image/AssetStreamer.h"
#include "brick/interfaces/display/TouchscreenTypes.h"
#include "brick/interfaces/display/WritablePixelBuffer.h"
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
constexpr char TAG[] = "brick_cyd_asset_fb";
constexpr std::uint16_t kWidth = 320;
constexpr std::uint16_t kHeight = 240;
constexpr std::uint16_t kStripeHeight = 16;
constexpr std::size_t kFrameBytes = static_cast<std::size_t>(kWidth) * kHeight * 2;
constexpr std::size_t kStripeBytes = static_cast<std::size_t>(kWidth) * kStripeHeight * 2;

brick::platform::esp32::Ili9341SpiDisplay display(
    brick::platform::esp32::profiles::cyd_ili9341_320x240());
brick::platform::esp32::touch::Xpt2046Touchscreen touch(
    brick::platform::esp32::profiles::cyd_xpt2046());
PartitionAssetSource asset_source("assets");
brick::core::image::AssetStreamer streamer(display);
std::uint8_t* framebuffers[2] = {};
std::uint8_t* stripe = nullptr;

bool load_asset(generated_assets::Id id, std::uint8_t* destination)
{
    const auto* asset = generated_assets::get(id);
    if (asset == nullptr)
        return false;
    const brick::interfaces::display::WritablePixelBuffer buffer{
        destination, kWidth, kHeight, static_cast<std::size_t>(kWidth) * 2,
        brick::interfaces::display::PixelFormat::rgb565, false};
    const auto started = esp_timer_get_time();
    if (!streamer.stream_to_buffer(*asset, asset_source, buffer, stripe, kStripeBytes))
        return false;
    ESP_LOGI(TAG, "loaded asset=%s into PSRAM framebuffer elapsed=%lldus",
             id == generated_assets::Id::joy_tears ? "joy_tears" : "sweat_smile",
             static_cast<long long>(esp_timer_get_time() - started));
    return true;
}

bool present(std::uint8_t* framebuffer)
{
    for (std::uint16_t y = 0; y < kHeight; y += kStripeHeight) {
        const auto height = (kHeight - y < kStripeHeight) ? kHeight - y : kStripeHeight;
        const auto bytes = static_cast<std::size_t>(height) * kWidth * 2;
        std::memcpy(stripe, framebuffer + static_cast<std::size_t>(y) * kWidth * 2, bytes);
        const brick::interfaces::display::PixelBuffer buffer{
            stripe, kWidth, static_cast<std::uint32_t>(height),
            static_cast<std::size_t>(kWidth) * 2,
            brick::interfaces::display::PixelFormat::rgb565, false};
        if (!display.draw_buffer({0, y, kWidth, height}, buffer))
            return false;
    }
    return true;
}
}  // namespace

extern "C" void app_main()
{
    ESP_LOGI(TAG, "Starting CYD asset partition + PSRAM framebuffer test");
    ESP_LOGI(TAG, "frame_bytes=%u double_buffer_bytes=%u free_psram=%u",
             static_cast<unsigned>(kFrameBytes), static_cast<unsigned>(2 * kFrameBytes),
             static_cast<unsigned>(heap_caps_get_free_size(MALLOC_CAP_SPIRAM)));

    framebuffers[0] = static_cast<std::uint8_t*>(heap_caps_malloc(kFrameBytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
    framebuffers[1] = static_cast<std::uint8_t*>(heap_caps_malloc(kFrameBytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
    stripe = static_cast<std::uint8_t*>(heap_caps_malloc(kStripeBytes, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL));
    if (framebuffers[0] == nullptr || framebuffers[1] == nullptr || stripe == nullptr) {
        ESP_LOGE(TAG, "framebuffer allocation failed");
        return;
    }
    if (!display.begin() || !touch.begin() || !asset_source.begin()) {
        ESP_LOGE(TAG, "display, touch or assets partition initialization failed");
        return;
    }

    auto selected = generated_assets::Id::joy_tears;
    std::uint8_t front = 0;
    if (!load_asset(selected, framebuffers[front]) || !present(framebuffers[front])) {
        ESP_LOGE(TAG, "initial asset presentation failed");
        return;
    }
    ESP_LOGI(TAG, "ready; touch toggles assets, assets are read from flash partition");

    brick::interfaces::display::TouchPoint point{};
    bool was_pressed = false;
    while (true) {
        std::size_t count = 0;
        const bool pressed = touch.read(&point, 1, count) && count != 0 &&
                             point.state != brick::interfaces::display::TouchState::released;
        if (pressed && !was_pressed) {
            selected = selected == generated_assets::Id::joy_tears
                           ? generated_assets::Id::sweat_smile
                           : generated_assets::Id::joy_tears;
            const auto back = static_cast<std::uint8_t>(front ^ 1U);
            if (load_asset(selected, framebuffers[back]) && present(framebuffers[back]))
                front = back;
            else
                ESP_LOGE(TAG, "asset presentation failed");
        }
        was_pressed = pressed;
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
