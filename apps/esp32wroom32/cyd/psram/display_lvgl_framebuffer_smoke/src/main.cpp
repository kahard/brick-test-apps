#include <cstddef>
#include <cstdint>
#include "brick/interfaces/display/TouchscreenTypes.h"
#include "brick/platform/esp32/Ili9341SpiDisplay.h"
#include "brick/platform/esp32/LvglDisplayAdapter.h"
#include "brick/platform/esp32/touch/Xpt2046Touchscreen.h"
#include "brick/platform/esp32/wroom32/profiles/cyd_ili9341_320x240.h"
#include "brick/platform/esp32/wroom32/profiles/cyd_xpt2046.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_partition.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lvgl.h"
#include "generated_assets.h"
LV_FONT_DECLARE(brick_roboto_20);
namespace {
constexpr char TAG[] = "brick_cyd_lvgl_fb"; constexpr std::size_t kImageBytes = 100U * 100U * 2U; constexpr std::size_t kFrameBytes = 320U * 240U * 2U;
brick::platform::esp32::Ili9341SpiDisplay display(brick::platform::esp32::profiles::cyd_ili9341_320x240()); brick::platform::esp32::touch::Xpt2046Touchscreen touch(brick::platform::esp32::profiles::cyd_xpt2046());
const esp_partition_t* assets_partition = nullptr; std::uint8_t* image_pixels = nullptr; lv_obj_t* image = nullptr; lv_obj_t* counter_label = nullptr; lv_image_dsc_t image_dsc{}; generated_assets::Id selected = generated_assets::Id::joy_tears; std::uint32_t counter = 0;
bool load_asset(generated_assets::Id id) { const auto* asset = generated_assets::find(id); return assets_partition && asset && asset->size == kImageBytes && esp_partition_read(assets_partition, asset->offset, image_pixels, asset->size) == ESP_OK; }
void read_touch(lv_indev_t*, lv_indev_data_t* data) { brick::interfaces::display::TouchPoint point{}; std::size_t count = 0; if (touch.read(&point, 1, count) && count && point.state != brick::interfaces::display::TouchState::released) { data->point = {point.x, point.y}; data->state = LV_INDEV_STATE_PRESSED; } else data->state = LV_INDEV_STATE_RELEASED; }
void clicked(lv_event_t*) { selected = selected == generated_assets::Id::joy_tears ? generated_assets::Id::sweat_smile : generated_assets::Id::joy_tears; if (load_asset(selected)) { ++counter; lv_image_set_src(image, &image_dsc); lv_label_set_text_fmt(counter_label, "Zmiana: %u", static_cast<unsigned>(counter)); ESP_LOGI(TAG, "partition asset switched"); } }
}
extern "C" void app_main() {
    ESP_LOGI(TAG, "Starting CYD PSRAM LVGL partition-asset test");
    image_pixels = static_cast<std::uint8_t*>(heap_caps_malloc(kImageBytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT)); assets_partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "assets");
    if (image_pixels == nullptr || assets_partition == nullptr) { ESP_LOGE(TAG, "asset buffer or assets partition unavailable"); return; }
    if (!image_pixels || !assets_partition || !display.begin() || !touch.begin() || !load_asset(selected)) { ESP_LOGE(TAG, "PSRAM, partition, display, touch or asset initialization failed"); return; }
    image_dsc = {.header = {.cf = LV_COLOR_FORMAT_RGB565, .w = 100, .h = 100, .stride = 200}, .data_size = kImageBytes, .data = image_pixels}; lv_init(); brick::platform::esp32::LvglDisplayAdapter adapter(display);
    auto* first = heap_caps_malloc(kFrameBytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT); auto* second = heap_caps_malloc(kFrameBytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT); if (first == nullptr || second == nullptr) { ESP_LOGE(TAG, "framebuffer allocation failed"); return; } if (!adapter.create(LV_DISPLAY_RENDER_MODE_FULL, first, second, kFrameBytes)) return;
    auto* indev = lv_indev_create(); lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); lv_indev_set_read_cb(indev, read_touch); auto* screen = lv_screen_active(); lv_obj_set_style_bg_color(screen, lv_color_hex(0x202040), 0);
    auto* title = lv_label_create(screen); lv_label_set_text(title, "CYD PSRAM LVGL ASSETS FLASH"); lv_obj_set_style_text_font(title, &brick_roboto_20, 0); lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0); lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 12);
    image = lv_image_create(screen); lv_image_set_src(image, &image_dsc); lv_obj_align(image, LV_ALIGN_TOP_MID, 0, 42); auto* button = lv_button_create(screen); lv_obj_set_size(button, 220, 58); lv_obj_align(button, LV_ALIGN_BOTTOM_MID, 0, -24); lv_obj_add_event_cb(button, clicked, LV_EVENT_CLICKED, nullptr);
    auto* label = lv_label_create(button); lv_label_set_text(label, "PRZELACZ"); lv_obj_set_style_text_font(label, &brick_roboto_20, 0); lv_obj_center(label); counter_label = lv_label_create(screen); lv_label_set_text(counter_label, "Asset: joy_tears"); lv_obj_set_style_text_font(counter_label, &brick_roboto_20, 0); lv_obj_align(counter_label, LV_ALIGN_BOTTOM_MID, 0, -88);
    ESP_LOGI(TAG, "LVGL initialized: partition assets, mode=FULL buffers=2"); while (true) { constexpr std::uint32_t step = 10; vTaskDelay(pdMS_TO_TICKS(step)); lv_tick_inc(step); lv_timer_handler(); }
}
