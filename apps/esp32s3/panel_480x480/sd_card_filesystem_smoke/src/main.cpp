#include <cerrno>
#include <cstdio>
#include <cstring>
#include <dirent.h>

#include "driver/sdspi_host.h"
#include "driver/spi_common.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdmmc_cmd.h"

namespace {
constexpr char kTag[] = "brick_sd_card_smoke";
constexpr char kMountPoint[] = "/sdcard";
constexpr char kTestPath[] = "/sdcard/brick_sd_test.bin";

// ESP32-S3 4" panel wiring from the vendor demo.
constexpr gpio_num_t kSdCs = GPIO_NUM_42;
constexpr gpio_num_t kSdMosi = GPIO_NUM_47;
constexpr gpio_num_t kSdMiso = GPIO_NUM_41;
constexpr gpio_num_t kSdSck = GPIO_NUM_48;

sdmmc_card_t* g_card = nullptr;
bool g_mounted = false;

bool mount_card() {
  if (g_mounted) return true;
  spi_bus_config_t bus = {};
  bus.mosi_io_num = kSdMosi;
  bus.miso_io_num = kSdMiso;
  bus.sclk_io_num = kSdSck;
  bus.quadwp_io_num = -1;
  bus.quadhd_io_num = -1;
  esp_err_t result = spi_bus_initialize(SPI2_HOST, &bus, SDSPI_DEFAULT_DMA);
  if (result != ESP_OK && result != ESP_ERR_INVALID_STATE) {
    ESP_LOGE(kTag, "SPI bus init failed: %s", esp_err_to_name(result));
    return false;
  }
  sdmmc_host_t host = SDSPI_HOST_DEFAULT();
  host.slot = SPI2_HOST;
  host.max_freq_khz = 1000;
  sdspi_device_config_t device = SDSPI_DEVICE_CONFIG_DEFAULT();
  device.host_id = SPI2_HOST;
  device.gpio_cs = kSdCs;
  esp_vfs_fat_mount_config_t mount = {};
  mount.format_if_mount_failed = false;
  mount.max_files = 4;
  mount.allocation_unit_size = 16 * 1024;
  result = esp_vfs_fat_sdspi_mount(kMountPoint, &host, &device, &mount, &g_card);
  if (result != ESP_OK) {
    ESP_LOGE(kTag, "SD mount failed: %s (0x%x)", esp_err_to_name(result), result);
    return false;
  }
  g_mounted = true;
  ESP_LOGI(kTag, "SD mounted at %s", kMountPoint);
  sdmmc_card_print_info(stdout, g_card);
  return true;
}

bool write_read_verify() {
  constexpr unsigned char expected[] = "BRICK SD CARD TEST 2026";
  constexpr std::size_t expected_size = sizeof(expected) - 1;
  {
    std::FILE* file = std::fopen(kTestPath, "wb");
    if (!file) { ESP_LOGE(kTag, "open for write failed: %s", std::strerror(errno)); return false; }
    const std::size_t written = std::fwrite(expected, 1, expected_size, file);
    std::fclose(file);
    if (written != expected_size) { ESP_LOGE(kTag, "short write: %u/%u", (unsigned)written, (unsigned)expected_size); return false; }
  }
  unsigned char actual[expected_size] = {};
  std::FILE* file = std::fopen(kTestPath, "rb");
  if (!file) { ESP_LOGE(kTag, "open for read failed: %s", std::strerror(errno)); return false; }
  const std::size_t read = std::fread(actual, 1, expected_size, file);
  std::fclose(file);
  const bool ok = read == expected_size && std::memcmp(actual, expected, expected_size) == 0;
  ESP_LOGI(kTag, "write/read/verify %s (read=%u expected=%u)", ok ? "OK" : "FAIL", (unsigned)read, (unsigned)expected_size);
  return ok;
}

void list_root() {
  DIR* directory = opendir(kMountPoint);
  if (!directory) {
    ESP_LOGW(kTag, "Unable to list %s", kMountPoint);
    return;
  }
  unsigned count = 0;
  while (dirent* entry = readdir(directory)) {
    if (std::strcmp(entry->d_name, ".") == 0 || std::strcmp(entry->d_name, "..") == 0) continue;
    ESP_LOGI(kTag, "ENTRY %u: %s%s", count++, entry->d_name,
             entry->d_type == DT_DIR ? "/" : "");
  }
  closedir(directory);
  ESP_LOGI(kTag, "Root entries: %u", count);
}
}  // namespace

extern "C" void app_main() {
  ESP_LOGI(kTag, "ESP32-S3 4\" SD card filesystem smoke");
  ESP_LOGI(kTag, "Pins CS=%d SCK=%d MOSI=%d MISO=%d", kSdCs, kSdSck, kSdMosi, kSdMiso);
  if (!mount_card()) { ESP_LOGE(kTag, "SD CARD TEST FAIL: mount"); return; }
  list_root();
  if (!write_read_verify()) { ESP_LOGE(kTag, "SD CARD TEST FAIL: write/read"); return; }
  ESP_LOGI(kTag, "SD CARD TEST PASS");
  while (true) vTaskDelay(pdMS_TO_TICKS(1000));
}
