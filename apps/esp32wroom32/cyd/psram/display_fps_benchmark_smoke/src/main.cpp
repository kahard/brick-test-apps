#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include "brick/interfaces/display/PixelBuffer.h"
#include "brick/platform/esp32/Ili9341SpiDisplay.h"
#include "brick/platform/esp32/wroom32/profiles/cyd_ili9341_320x240.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace
{
constexpr char          TAG[]            = "brick_cyd_fps";
constexpr std::uint16_t kWidth           = 320;
constexpr std::uint16_t kHeight          = 240;
constexpr std::uint16_t kStripeHeight    = 16;
constexpr std::size_t   kBytesPerPixel   = 2;
constexpr std::size_t   kFrameBytes      = static_cast<std::size_t>(kWidth) * kHeight * kBytesPerPixel;
constexpr std::size_t   kStripeBytes     = static_cast<std::size_t>(kWidth) * kStripeHeight * kBytesPerPixel;
constexpr std::uint32_t kBenchmarkFrames = 120;

brick::platform::esp32::Ili9341SpiDisplay display(brick::platform::esp32::profiles::cyd_ili9341_320x240());
std::uint8_t*                             framebuffers[2] = {};
std::uint8_t*                             stripe          = nullptr;

void fill_frame(std::uint8_t* framebuffer, std::uint16_t color)
{
    for (std::size_t offset = 0; offset < kFrameBytes; offset += 2)
    {
        framebuffer[offset]     = static_cast<std::uint8_t>(color & 0xFF);
        framebuffer[offset + 1] = static_cast<std::uint8_t>(color >> 8);
    }
}

bool present(std::uint8_t* framebuffer, std::int64_t& transfer_us)
{
    const auto started = esp_timer_get_time();
    for (std::uint16_t y = 0; y < kHeight; y += kStripeHeight)
    {
        const auto height = std::min<std::uint16_t>(kStripeHeight, kHeight - y);
        const auto bytes  = static_cast<std::size_t>(height) * kWidth * kBytesPerPixel;
        std::memcpy(stripe, framebuffer + static_cast<std::size_t>(y) * kWidth * kBytesPerPixel, bytes);
        const brick::interfaces::display::PixelBuffer buffer{ stripe,
                                                              kWidth,
                                                              height,
                                                              static_cast<std::size_t>(kWidth) * kBytesPerPixel,
                                                              brick::interfaces::display::PixelFormat::rgb565,
                                                              false };
        if (!display.draw_buffer({ 0, y, kWidth, height }, buffer))
            return false;
        // The polling SPI transfer can occupy CPU0 for several seconds over
        // a benchmark run. Let the idle task run so the task watchdog remains
        // meaningful without adding a full scheduler tick to each frame.
        taskYIELD();
    }
    transfer_us = esp_timer_get_time() - started;
    return true;
}

void benchmark()
{
    const std::uint16_t colors[]          = { 0xF800, 0x001F };  // red, blue in RGB565
    std::uint8_t        front             = 0;
    std::int64_t        render_total      = 0;
    std::int64_t        transfer_total    = 0;
    const auto          benchmark_started = esp_timer_get_time();

    for (std::uint32_t frame = 0; frame < kBenchmarkFrames; ++frame)
    {
        const auto render_started = esp_timer_get_time();
        fill_frame(framebuffers[front ^ 1U], colors[frame & 1U]);
        render_total += esp_timer_get_time() - render_started;

        std::int64_t transfer_us = 0;
        if (!present(framebuffers[front ^ 1U], transfer_us))
        {
            ESP_LOGE(TAG, "frame transfer failed at frame=%u", static_cast<unsigned>(frame + 1));
            return;
        }
        transfer_total += transfer_us;
        front ^= 1U;
    }

    const auto elapsed = esp_timer_get_time() - benchmark_started;
    ESP_LOGI(TAG, "benchmark frames=%u elapsed=%lldus fps=%.2f avg_render=%.2fus avg_transfer=%.2fus",
             static_cast<unsigned>(kBenchmarkFrames), static_cast<long long>(elapsed),
             static_cast<double>(kBenchmarkFrames) * 1000000.0 / static_cast<double>(elapsed),
             static_cast<double>(render_total) / kBenchmarkFrames,
             static_cast<double>(transfer_total) / kBenchmarkFrames);
}
}  // namespace

extern "C" void app_main()
{
    ESP_LOGI(TAG, "Starting CYD PSRAM full-frame FPS benchmark: %ux%u frame_bytes=%u", kWidth, kHeight,
             static_cast<unsigned>(kFrameBytes));
    ESP_LOGI(TAG, "free_psram_before=%u", static_cast<unsigned>(heap_caps_get_free_size(MALLOC_CAP_SPIRAM)));

    framebuffers[0] = static_cast<std::uint8_t*>(heap_caps_malloc(kFrameBytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
    framebuffers[1] = static_cast<std::uint8_t*>(heap_caps_malloc(kFrameBytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
    stripe          = static_cast<std::uint8_t*>(heap_caps_malloc(kStripeBytes, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL));
    if (framebuffers[0] == nullptr || framebuffers[1] == nullptr || stripe == nullptr)
    {
        ESP_LOGE(TAG, "allocation failed: requires CYD variant with PSRAM");
        return;
    }
    if (!display.begin())
    {
        ESP_LOGE(TAG, "ILI9341 initialization failed");
        return;
    }
    ESP_LOGI(TAG, "buffers_ready free_psram_after=%u",
             static_cast<unsigned>(heap_caps_get_free_size(MALLOC_CAP_SPIRAM)));

    while (true)
    {
        benchmark();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
