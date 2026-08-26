#include <cstddef>
#include <cstdint>

#include "brick/interfaces/display/IFrameBufferDisplay.h"
#include "brick/platform/esp32/s3/profiles/st7701s_480x480.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace {
constexpr char TAG[] = "brick_st7701s_benchmark";
constexpr std::uint16_t kWidth = 480;
constexpr std::uint16_t kHeight = 480;
constexpr std::uint32_t kReportFrames = 60;

auto panel_config() {
  auto config = brick::platform::esp32::s3::profiles::st7701s_480x480();
  config.frame_buffer_count = 2;
  return config;
}

brick::platform::esp32::s3::St7701sRgbDisplay display(panel_config());

bool render_frame(brick::interfaces::display::IFrameBufferDisplay& framebuffers,
                  std::uint8_t index, std::uint32_t frame_number,
                  std::int64_t& render_us) {
  brick::interfaces::display::WritablePixelBuffer buffer;
  if (!framebuffers.get_frame_buffer(index, buffer))
    return false;

  const auto start_us = esp_timer_get_time();
  auto* pixels = reinterpret_cast<volatile std::uint16_t*>(buffer.data);
  const auto color = (frame_number & 1U) == 0U ? 0xF800U : 0x001FU;
  for (std::size_t pixel = 0; pixel < kWidth * kHeight; ++pixel)
    pixels[pixel] = static_cast<std::uint16_t>(color);
  render_us = esp_timer_get_time() - start_us;

  return framebuffers.present_frame_buffer(index) &&
         display.wait_for_transfer_complete(1000) &&
         display.wait_for_vsync(1000);
}
}  // namespace

extern "C" void app_main() {
  ESP_LOGI(TAG, "Starting ESP32-S3 ST7701S framebuffer benchmark");
  if (!display.begin()) {
    ESP_LOGE(TAG, "ST7701S RGB display initialization failed");
    return;
  }

  auto& framebuffers = static_cast<brick::interfaces::display::IFrameBufferDisplay&>(display);
  const auto capabilities = display.capabilities();
  if (!capabilities.dma || !capabilities.vsync || !capabilities.scanout_buffers || capabilities.max_buffer_count < 2) {
    ESP_LOGE(TAG, "DMA scanout capability check failed: dma=%d vsync=%d scanout=%d buffers=%u",
             capabilities.dma, capabilities.vsync, capabilities.scanout_buffers, capabilities.max_buffer_count);
    return;
  }
  ESP_LOGI(TAG, "Display initialized, framebuffer count=%u",
           framebuffers.frame_buffer_count());
  ESP_LOGI(TAG, "DMA scanout ready: dma=%d dma_alignment=%u buffers=%u vsync=%d",
           capabilities.dma, static_cast<unsigned>(capabilities.dma_alignment_bytes),
           capabilities.max_buffer_count, capabilities.vsync);
  if (framebuffers.frame_buffer_count() < 2) {
    ESP_LOGE(TAG, "Two framebuffers are required for this benchmark");
    return;
  }

  std::uint8_t buffer_index = 0;
  std::uint32_t frame_number = 0;
  std::uint32_t report_frame = 0;
  std::int64_t report_start_us = esp_timer_get_time();
  std::int64_t render_total_us = 0;

  while (true) {
    std::int64_t render_us = 0;
    if (!render_frame(framebuffers, buffer_index, frame_number, render_us)) {
      ESP_LOGE(TAG, "Framebuffer %u presentation failed", buffer_index);
      return;
    }
    render_total_us += render_us;
    ++frame_number;
    ++report_frame;
    buffer_index ^= 1;

    if (report_frame == kReportFrames) {
      const auto elapsed_us = esp_timer_get_time() - report_start_us;
      const auto fps = static_cast<double>(report_frame) * 1'000'000.0 / elapsed_us;
      const auto average_render_us = render_total_us / report_frame;
      ESP_LOGI(TAG, "frames=%u elapsed=%lldus fps=%.2f avg_render=%lldus",
               report_frame, static_cast<long long>(elapsed_us), fps,
               static_cast<long long>(average_render_us));
      report_frame = 0;
      report_start_us = esp_timer_get_time();
      render_total_us = 0;
    }
  }
}
