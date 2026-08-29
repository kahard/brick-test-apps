#include <cstdint>
#include <cstring>

#include "generated_assets.h"
#include "brick/core/image/AssetBundle.h"

#include "brick/interfaces/display/IFrameBufferDisplay.h"
#include "brick/platform/esp32/s3/profiles/st7701s_480x480.h"
#include "esp_log.h"
#include "esp_timer.h"

namespace {
constexpr char TAG[] = "brick_st7701s_asset_fb";
constexpr std::uint16_t kWidth = 480;
constexpr std::uint16_t kHeight = 480;
constexpr std::size_t kFrameBytes = kWidth * kHeight * 2;

brick::core::image::AssetBundle assets = generated_assets::bundle();

auto panel_config() {
  auto config = brick::platform::esp32::s3::profiles::st7701s_480x480();
  config.pixel_clock_hz = 12'000'000;
  config.frame_buffer_count = 2;
  return config;
}

brick::platform::esp32::s3::St7701sRgbDisplay display(panel_config());

bool copy_asset(const brick::interfaces::display::ImageAsset& image,
                brick::interfaces::display::WritablePixelBuffer& target) {
  if (image.data_size != kFrameBytes || target.data == nullptr ||
      target.stride_bytes != kWidth * 2 || target.width != kWidth || target.height != kHeight) {
    ESP_LOGE(TAG, "Invalid asset or framebuffer description");
    return false;
  }
  std::memcpy(target.data, image.data, kFrameBytes);
  return true;
}
}  // namespace

extern "C" void app_main() {
  ESP_LOGI(TAG, "Starting ST7701S asset framebuffer page-flip test: %ux%u pclk=12MHz",
           kWidth, kHeight);
  if (!display.begin()) {
    ESP_LOGE(TAG, "ST7701S RGB display initialization failed");
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

  const brick::interfaces::display::ImageAsset joy_tears = assets.image(static_cast<std::size_t>(generated_assets::Id::joy_tears));
  if (!copy_asset(joy_tears, framebuffer[0]) ||
      !copy_asset(joy_tears, framebuffer[1]) ||
      !framebuffers.present_frame_buffer(0)) {
    ESP_LOGE(TAG, "Unable to initialize framebuffer page flip");
    return;
  }

  std::uint8_t active = 0;
  std::uint32_t frame = 0;
  std::uint32_t benchmark_frames = 0;
  std::int64_t benchmark_started = esp_timer_get_time();
  ESP_LOGI(TAG, "Full framebuffer page flip active: framebuffer_bytes=%u",
           static_cast<unsigned>(kFrameBytes));

  while (true) {
    const auto frame_started = esp_timer_get_time();
    const std::uint8_t back = active ^ 1U;
    const bool use_sweat_smile = (frame & 1U) != 0U;
    const generated_assets::Id asset_id = use_sweat_smile ? generated_assets::Id::sweat_smile : generated_assets::Id::joy_tears;
    const brick::interfaces::display::ImageAsset asset = assets.image(static_cast<std::size_t>(asset_id));

    if (!copy_asset(asset, framebuffer[back]) ||
        !display.wait_for_vsync(100) || !framebuffers.present_frame_buffer(back)) {
      ESP_LOGE(TAG, "Framebuffer page flip failed at frame=%u",
               static_cast<unsigned>(frame));
      return;
    }

    active = back;
    ++frame;
    ++benchmark_frames;
    ESP_LOGI(TAG, "presented frame=%u asset=%s copy+present=%lldus",
             static_cast<unsigned>(frame), use_sweat_smile ? "sweat_smile" : "joy_tears",
             esp_timer_get_time() - frame_started);

    if (benchmark_frames == 60) {
      const auto elapsed = esp_timer_get_time() - benchmark_started;
      ESP_LOGI(TAG, "page-flip benchmark: frames=60 elapsed=%lldus fps=%.2f", elapsed,
               60000000.0 / static_cast<double>(elapsed));
      benchmark_frames = 0;
      benchmark_started = esp_timer_get_time();
    }
  }
}
