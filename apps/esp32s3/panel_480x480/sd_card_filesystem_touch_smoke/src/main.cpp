#include <array>
#include <algorithm>

#include "brick/boards/esp32/s3/Panel480Board.h"
#include "brick/interfaces/display/PixelBuffer.h"
#include "brick/core/storage/StorageWriteVerify.h"
#include "brick/platform/esp32/SdSpiFileSystem.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "generated/generated_font.h"
#include "generated/generated_font_large.h"

namespace {
constexpr char kTag[] = "brick_sd_card_smoke";
constexpr char kMountPoint[] = "/sdcard";
// Keep the filename in FAT 8.3 form; this panel's FAT configuration does not
// accept long filenames.
constexpr char kTestPath[] = "/sdcard/BRICKTST.BIN";

// ESP32-S3 4" panel wiring from the vendor demo.
constexpr gpio_num_t kSdCs = GPIO_NUM_42;
constexpr gpio_num_t kSdMosi = GPIO_NUM_47;
constexpr gpio_num_t kSdMiso = GPIO_NUM_41;
constexpr gpio_num_t kSdSck = GPIO_NUM_48;

brick::platform::esp32::s3::Panel480Board g_board;

void show_status(std::uint16_t color, const char* message) {
  constexpr std::uint16_t kStatusHeight = 96;
  constexpr std::uint16_t kTextTop = 8;
  static std::uint16_t pixels[480 * kStatusHeight];
  std::fill(std::begin(pixels), std::end(pixels), 0x0000);
  const auto render_line = [&](const char* text, const char* chars,
                               const BrickBitmapGlyph* glyphs, std::size_t count,
                               std::uint16_t top) {
    std::uint16_t x = 8;
    for (const char* c = text; *c && x < 472; ++c) {
      std::size_t index = 0;
      while (index < count && chars[index] != *c) ++index;
      if (index >= count) continue;
      const BrickBitmapGlyph& glyph = glyphs[index];
      for (std::uint16_t y = 0; y < glyph.height && y + top < kStatusHeight; ++y)
        for (std::uint16_t col = 0; col < glyph.width && x + col < 480; ++col)
          if (glyph.data[y * glyph.stride + col / 8] & (0x80u >> (col & 7)))
            pixels[(y + top) * 480 + x + col] = color;
      x = static_cast<std::uint16_t>(x + glyph.width + 1);
    }
  };
  render_line(message, brick_roboto_20_chars, brick_roboto_20_glyphs,
              brick_roboto_20_count, kTextTop);
  render_line("FONT 28 PX", brick_roboto_28_chars, brick_roboto_28_glyphs,
              brick_roboto_28_count, 42);
  const brick::interfaces::display::PixelBuffer buffer{
      reinterpret_cast<const std::uint8_t*>(pixels), 480, kStatusHeight, 960,
      brick::interfaces::display::PixelFormat::rgb565, false};
  // The area must exactly match the PixelBuffer dimensions.  A mismatch makes
  // the display driver reject the transfer silently (and leaves the panel
  // showing only its backlight).
  const bool submitted = g_board.display().draw_buffer({0, 0, 480, kStatusHeight}, buffer);
  const bool completed = submitted && g_board.display().wait_for_transfer_complete(1000);
  ESP_LOGI(kTag, "status '%s': draw=%d complete=%d", message, submitted ? 1 : 0,
           completed ? 1 : 0);
}

}  // namespace

extern "C" void app_main() {
  ESP_LOGI(kTag, "ESP32-S3 4\" SD card filesystem smoke");
  if (!g_board.begin()) { ESP_LOGE(kTag, "Board init failed"); return; }
  show_status(0x001F, "SD INIT");
  ESP_LOGI(kTag, "Pins CS=%d SCK=%d MOSI=%d MISO=%d", kSdCs, kSdSck, kSdMosi, kSdMiso);
  if (!g_board.sd_card().mount()) { show_status(0xF800, "SD MOUNT FAIL"); ESP_LOGE(kTag, "SD CARD TEST FAIL: mount"); return; }
  const std::vector<std::string> files = g_board.sd_card().list_files(kMountPoint);
  ESP_LOGI(kTag, "Root files: %u", static_cast<unsigned>(files.size()));
  constexpr std::uint8_t pattern[] = "BRICK SD CARD TEST 2026";
  if (!brick::core::storage::write_verify(g_board.sd_card(), kTestPath, pattern, sizeof(pattern) - 1U)) { show_status(0xF800, "WRITE READ FAIL"); ESP_LOGE(kTag, "SD CARD TEST FAIL: write/read"); }
  else show_status(0x07E0, "SD READY");
  ESP_LOGI(kTag, "SD CARD TEST PASS");
  std::array<brick::interfaces::display::TouchPoint, 5> points{};
  bool touch_was_down = false;
  TickType_t next_card_probe = xTaskGetTickCount();
  while (true) {
    if (xTaskGetTickCount() >= next_card_probe) {
      next_card_probe = xTaskGetTickCount() + pdMS_TO_TICKS(1000);
      if (g_board.sd_card().mounted()) {
        if (!g_board.sd_card().probe(kTestPath)) {
          g_board.sd_card().unmount();
          show_status(0xF800, "SD REMOVED");
        }
      } else if (g_board.sd_card().mount()) {
        const std::vector<std::string> files = g_board.sd_card().list_files(kMountPoint);
        ESP_LOGI(kTag, "Root files: %u", static_cast<unsigned>(files.size()));
        show_status(0x07E0, "SD INSERTED");
      }
    }
    std::size_t count = 0;
    const bool touch_down = g_board.touch().read(points.data(), points.size(), count) && count > 0;
    if (touch_down && !touch_was_down) {
      if (brick::core::storage::write_verify(g_board.sd_card(), kTestPath, pattern, sizeof(pattern) - 1U)) show_status(0x07E0, "WRITE READ OK"); else show_status(0xF800, "WRITE READ FAIL");
    }
    touch_was_down = touch_down;
    vTaskDelay(pdMS_TO_TICKS(30));
  }
}
