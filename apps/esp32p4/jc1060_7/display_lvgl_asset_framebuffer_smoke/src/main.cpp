#include <cstddef>
#include <cstdint>
#include "brick/platform/esp32/LvglDisplayAdapter.h"
#include "brick/platform/esp32/LvglTouchAdapter.h"
#include "brick/boards/esp32/p4/Jc1060Board.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_partition.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "generated_assets.h"
#include "lvgl.h"

namespace {
constexpr char TAG[] = "brick_jc1060_lvgl";
constexpr std::size_t kWidth = 1024, kHeight = 600;
constexpr std::size_t kFrameBytes = kWidth * kHeight * 2;
constexpr std::size_t kImageBytes = 100 * 100 * 2;
brick::platform::esp32::p4::Jc1060Board board;
auto& display = board.display();
auto& touch = board.touch();
const esp_partition_t* assets_partition = nullptr;
std::uint8_t* image_pixels = nullptr;
lv_obj_t* status_label = nullptr;
lv_obj_t* image_widget = nullptr;
lv_image_dsc_t image_dsc{};
std::uint32_t touches = 0;
bool showing_joy_tears = true;

bool load_asset(generated_assets::Id id) {
    const auto* asset = generated_assets::get(id);
    return assets_partition && image_pixels && asset && asset->size == kImageBytes && esp_partition_read(assets_partition, asset->offset, image_pixels, asset->size) == ESP_OK;
}
void clicked(lv_event_t*) {
    ++touches;
    showing_joy_tears = !showing_joy_tears;
    if (load_asset(showing_joy_tears ? generated_assets::Id::joy_tears : generated_assets::Id::sweat_smile)) {
        lv_image_set_src(image_widget, &image_dsc);
        lv_label_set_text_fmt(status_label, "%s | Dotkniecia: %u", showing_joy_tears ? "joy_tears" : "sweat_smile", static_cast<unsigned>(touches));
    }
    ESP_LOGI(TAG, "LVGL button clicked count=%u", static_cast<unsigned>(touches));
}
}  // namespace

extern "C" void app_main() {
    ESP_LOGI(TAG, "Starting JC1060 7-inch LVGL full-frame asset test");
    assets_partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "assets");
    constexpr auto kDmaPsramCaps = MALLOC_CAP_SPIRAM | MALLOC_CAP_DMA | MALLOC_CAP_8BIT;
    image_pixels = static_cast<std::uint8_t*>(heap_caps_malloc(kImageBytes, kDmaPsramCaps));
    auto* first = heap_caps_malloc(kFrameBytes, kDmaPsramCaps);
    auto* second = heap_caps_malloc(kFrameBytes, kDmaPsramCaps);
    if (!assets_partition || !image_pixels || !first || !second) { ESP_LOGE(TAG, "asset partition or PSRAM allocation failed"); return; }
    if (!board.begin() || !load_asset(generated_assets::Id::joy_tears)) { ESP_LOGE(TAG, "board or asset initialization failed"); return; }
    image_dsc = {.header = {.cf = LV_COLOR_FORMAT_RGB565, .w = 100, .h = 100, .stride = 200}, .data_size = kImageBytes, .data = image_pixels};
    lv_init(); brick::platform::esp32::LvglDisplayAdapter adapter(display);
    if (!adapter.create(LV_DISPLAY_RENDER_MODE_FULL, first, second, kFrameBytes)) { ESP_LOGE(TAG, "LVGL display creation failed"); return; }
    brick::platform::esp32::LvglTouchAdapter touch_adapter(touch);
    if (touch_adapter.create() == nullptr) { ESP_LOGE(TAG, "LVGL touch input creation failed"); return; }
    auto* screen = lv_screen_active(); lv_obj_set_style_bg_color(screen, lv_color_hex(0x202040), 0);
    auto* title = lv_label_create(screen); lv_label_set_text(title, "JC1060 LVGL ASSET"); lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 24);
    image_widget = lv_image_create(screen); lv_image_set_src(image_widget, &image_dsc); lv_obj_align(image_widget, LV_ALIGN_CENTER, 0, -40);
    auto* button = lv_button_create(screen); lv_obj_set_size(button, 280, 70); lv_obj_align(button, LV_ALIGN_BOTTOM_MID, 0, -30); lv_obj_add_event_cb(button, clicked, LV_EVENT_CLICKED, nullptr);
    auto* button_label = lv_label_create(button); lv_label_set_text(button_label, "DOTKNIJ"); lv_obj_center(button_label);
    status_label = lv_label_create(screen); lv_label_set_text(status_label, "Asset z partycji flash"); lv_obj_align(status_label, LV_ALIGN_BOTTOM_MID, 0, -110);
    ESP_LOGI(TAG, "LVGL initialized: mode=FULL framebuffer_bytes=%u asset_bytes=%u", static_cast<unsigned>(kFrameBytes), static_cast<unsigned>(kImageBytes));
    lv_refr_now(lv_display_get_default());
    while (true) { vTaskDelay(pdMS_TO_TICKS(10)); lv_tick_inc(10); lv_timer_handler(); }
}
