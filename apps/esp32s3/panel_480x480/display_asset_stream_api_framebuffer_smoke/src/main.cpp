#include <array>
#include <cstdint>

#include "generated_assets.h"
#include "PartitionAssetSource.h"
#include "brick/core/image/AssetStreamer.h"
#include "brick/interfaces/display/IFrameBufferDisplay.h"
#include "brick/interfaces/display/ITouchscreen.h"
#include "brick/platform/esp32/s3/profiles/st7701s_480x480.h"
#include "brick/platform/esp32/s3/profiles/st7701s_gt911.h"
#include "esp_log.h"
#include "esp_timer.h"

namespace {
constexpr char TAG[] = "brick_st7701s_asset_api_fb";
constexpr std::uint16_t kWidth = 480;
constexpr std::uint16_t kHeight = 480;
constexpr std::uint16_t kStripeHeight = 20;
constexpr std::size_t kScratchBytes = static_cast<std::size_t>(kWidth) * kStripeHeight * 2;

auto panel_config() {
    auto config = brick::platform::esp32::s3::profiles::st7701s_480x480();
    config.pixel_clock_hz = 12'000'000;
    config.frame_buffer_count = 2;
    return config;
}

brick::platform::esp32::s3::St7701sRgbDisplay display(panel_config());
brick::platform::esp32::touch::Gt911Touchscreen touch(
    brick::platform::esp32::s3::profiles::st7701s_gt911());
std::array<std::uint8_t, kScratchBytes> scratch{};

PartitionAssetSource asset_source("assets");
brick::core::image::AssetStreamer streamer(display);
}  // namespace

extern "C" void app_main() {
    ESP_LOGI(TAG, "Starting ESP32-S3 partition AssetStreamer framebuffer test: %ux%u pclk=12MHz",
             kWidth, kHeight);
    if (!display.begin()) {
        ESP_LOGE(TAG, "ST7701S RGB display initialization failed");
        return;
    }
    if (!asset_source.begin()) {
        ESP_LOGE(TAG, "Assets partition not found");
        return;
    }
    if (!touch.begin()) {
        ESP_LOGE(TAG, "GT911 initialization failed");
        return;
    }

    auto& framebuffers = static_cast<brick::interfaces::display::IFrameBufferDisplay&>(display);
    if (framebuffers.frame_buffer_count() != 2) {
        ESP_LOGE(TAG, "Expected two framebuffers, got %u", framebuffers.frame_buffer_count());
        return;
    }

    brick::interfaces::display::WritablePixelBuffer framebuffer[2];
    if (!framebuffers.get_frame_buffer(0, framebuffer[0]) ||
        !framebuffers.get_frame_buffer(1, framebuffer[1])) {
        ESP_LOGE(TAG, "Unable to acquire both framebuffers");
        return;
    }

    const auto* initial_asset = generated_assets::get(generated_assets::Id::joy_tears);
    if (initial_asset == nullptr ||
        !streamer.stream_to_buffer(*initial_asset, asset_source, framebuffer[0],
                                   scratch.data(), scratch.size()) ||
        !streamer.stream_to_buffer(*initial_asset, asset_source, framebuffer[1],
                                   scratch.data(), scratch.size()) ||
        !framebuffers.present_frame_buffer(0)) {
        ESP_LOGE(TAG, "Unable to initialize asset framebuffer");
        return;
    }

    std::uint8_t active = 0;
    std::uint32_t frame = 0;
    std::uint32_t benchmark_frames = 0;
    std::int64_t benchmark_started = esp_timer_get_time();
    bool background_mode = false;
    bool touch_down = false;
    ESP_LOGI(TAG, "Partition AssetStreamer page-flip test active: touch toggles images/backgrounds");

    while (true) {
        const auto frame_started = esp_timer_get_time();
        const std::uint8_t back = active ^ 1U;
        const bool second_asset = (frame & 1U) != 0U;
        brick::interfaces::display::TouchPoint point{};
        std::size_t touch_count = 0;
        const bool has_touch = touch.read(&point, 1, touch_count) && touch_count > 0 &&
                               point.state != brick::interfaces::display::TouchState::released;
        if (has_touch && !touch_down) {
            background_mode = !background_mode;
            ESP_LOGI(TAG, "touch toggled asset set=%s at x=%d y=%d",
                     background_mode ? "solid backgrounds" : "smile images",
                     point.x, point.y);
        }
        touch_down = has_touch;

        const auto selected_id = background_mode
            ? (second_asset ? generated_assets::Id::blue_background
                            : generated_assets::Id::red_background)
            : (second_asset ? generated_assets::Id::sweat_smile
                            : generated_assets::Id::joy_tears);
        const auto* asset = generated_assets::get(selected_id);
        if (asset == nullptr ||
            !streamer.stream_to_buffer(*asset, asset_source, framebuffer[back],
                                       scratch.data(), scratch.size()) ||
            !display.wait_for_vsync(100) ||
            !framebuffers.present_frame_buffer(back)) {
            ESP_LOGE(TAG, "Asset framebuffer page flip failed at frame=%u",
                     static_cast<unsigned>(frame));
            return;
        }

        active = back;
        ++frame;
        ++benchmark_frames;
        ESP_LOGI(TAG, "presented frame=%u asset=%u mode=%s partition-stream+flip=%lldus",
                 static_cast<unsigned>(frame), static_cast<unsigned>(selected_id),
                 background_mode ? "backgrounds" : "smiles",
                 esp_timer_get_time() - frame_started);
        if (benchmark_frames == 60) {
            const auto elapsed = esp_timer_get_time() - benchmark_started;
            ESP_LOGI(TAG, "asset framebuffer benchmark: frames=60 elapsed=%lldus fps=%.2f",
                     elapsed, 60000000.0 / static_cast<double>(elapsed));
            benchmark_frames = 0;
            benchmark_started = esp_timer_get_time();
        }
    }
}
