#include <cstddef>
#include <cstdint>
#include "brick/platform/esp32/Ili9341SpiDisplay.h"
#include "brick/platform/esp32/LvglDisplayAdapter.h"
#include "brick/platform/esp32/LvglTouchAdapter.h"
#include "brick/platform/esp32/touch/Xpt2046Touchscreen.h"
#include "brick/platform/esp32/wroom32/profiles/cyd_ili9341_320x240.h"
#include "brick/platform/esp32/wroom32/profiles/cyd_xpt2046.h"
#include "esp_log.h"
#include "esp_partition.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lvgl.h"
#include "generated_assets.h"
LV_FONT_DECLARE(brick_roboto_20);
namespace {
constexpr char TAG[] = "brick_cyd_lvgl";
constexpr std::size_t kImageBytes = 100U * 100U * 2U;
alignas(4) std::uint8_t draw_buffer[320U * 32U * 2U];
alignas(4) std::uint8_t image_pixels[kImageBytes];
brick::platform::esp32::Ili9341SpiDisplay display(brick::platform::esp32::profiles::cyd_ili9341_320x240());
brick::platform::esp32::touch::Xpt2046Touchscreen touch(brick::platform::esp32::profiles::cyd_xpt2046());
const esp_partition_t* assets_partition = nullptr;
lv_obj_t* image = nullptr; lv_obj_t* counter_label = nullptr;
lv_image_dsc_t image_dsc{
    .header = {.cf = LV_COLOR_FORMAT_RGB565, .w = 100, .h = 100, .stride = 200},
    .data_size = kImageBytes,
    .data = image_pixels,
};
generated_assets::Id selected = generated_assets::Id::joy_tears; std::uint32_t counter = 0;
bool load_asset(generated_assets::Id id) { const auto* asset = generated_assets::get(id); return assets_partition && asset && asset->size == kImageBytes && esp_partition_read(assets_partition, asset->offset, image_pixels, asset->size) == ESP_OK; }
void clicked(lv_event_t*) { selected = selected == generated_assets::Id::joy_tears ? generated_assets::Id::sweat_smile : generated_assets::Id::joy_tears; if (load_asset(selected)) { ++counter; lv_image_set_src(image, &image_dsc); lv_label_set_text_fmt(counter_label, "Zmiana: %u", static_cast<unsigned>(counter)); ESP_LOGI(TAG, "partition asset switched"); } }
}
extern "C" void app_main() {
    ESP_LOGI(TAG, "Starting CYD no-PSRAM LVGL partition-asset test");
    assets_partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "assets");
    if (!assets_partition || !display.begin() || !touch.begin() || !load_asset(selected)) { ESP_LOGE(TAG, "partition, display, touch or asset initialization failed"); return; }
    lv_init(); brick::platform::esp32::LvglDisplayAdapter adapter(display); if (!adapter.create(LV_DISPLAY_RENDER_MODE_PARTIAL, draw_buffer, nullptr, sizeof(draw_buffer))) return;
    brick::platform::esp32::LvglTouchAdapter touch_adapter(touch);
    if (touch_adapter.create() == nullptr) { ESP_LOGE(TAG, "LVGL touch input creation failed"); return; }
    auto* screen = lv_screen_active(); lv_obj_set_style_bg_color(screen, lv_color_hex(0x202040), 0);
    auto* title = lv_label_create(screen); lv_label_set_text(title, "CYD LVGL ASSETS FLASH"); lv_obj_set_style_text_font(title, &brick_roboto_20, 0); lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0); lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 12);
    image = lv_image_create(screen); lv_image_set_src(image, &image_dsc); lv_obj_align(image, LV_ALIGN_TOP_MID, 0, 42);
    auto* button = lv_button_create(screen); lv_obj_set_size(button, 220, 58); lv_obj_align(button, LV_ALIGN_BOTTOM_MID, 0, -24); lv_obj_add_event_cb(button, clicked, LV_EVENT_CLICKED, nullptr);
    auto* label = lv_label_create(button); lv_label_set_text(label, "PRZELACZ"); lv_obj_set_style_text_font(label, &brick_roboto_20, 0); lv_obj_center(label);
    counter_label = lv_label_create(screen); lv_label_set_text(counter_label, "Asset: joy_tears"); lv_obj_set_style_text_font(counter_label, &brick_roboto_20, 0); lv_obj_align(counter_label, LV_ALIGN_BOTTOM_MID, 0, -88);
    ESP_LOGI(TAG, "LVGL initialized: partition assets, mode=PARTIAL");
    while (true) { constexpr std::uint32_t step = 10; vTaskDelay(pdMS_TO_TICKS(step)); lv_tick_inc(step); lv_timer_handler(); }
}
