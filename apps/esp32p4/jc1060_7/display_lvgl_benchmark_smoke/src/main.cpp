#include <cstddef>
#include <cstdint>
#include "brick/platform/esp32/LvglDisplayAdapter.h"
#include "brick/platform/esp32/p4/MipiDsiDisplay.h"
#include "brick/platform/esp32/p4/profiles/guition_jc1060p470c_i_w.h"
#include "driver/gpio.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_partition.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "generated_assets.h"
#include "lvgl.h"

namespace {
constexpr char TAG[] = "brick_jc1060_lvgl_benchmark";
constexpr std::size_t W = 1024, H = 600, FRAME_BYTES = W * H * 2, IMAGE_BYTES = 100 * 100 * 2;
brick::platform::esp32::p4::MipiDsiDisplay display(brick::platform::esp32::p4::profiles::guition_jc1060p470c_i_w());
const esp_partition_t* assets = nullptr;
std::uint8_t* image_joy_tears = nullptr;
std::uint8_t* image_sweat_smile = nullptr;
lv_image_dsc_t image_dsc{};
lv_obj_t* image_widget = nullptr;

bool load(generated_assets::Id id, std::uint8_t* target) {
    const auto* a = generated_assets::find(id);
    return assets && target && a && a->size == IMAGE_BYTES && esp_partition_read(assets, a->offset, target, a->size) == ESP_OK;
}

void run_asset_benchmark(lv_display_t* d) {
    constexpr std::uint32_t count = 60;
    const auto start = esp_timer_get_time();
    for (std::uint32_t i = 0; i < count; ++i) {
        image_dsc.data = (i & 1) ? image_sweat_smile : image_joy_tears;
        lv_image_set_src(image_widget, &image_dsc);
        lv_refr_now(d);
    }
    const auto elapsed = esp_timer_get_time() - start;
    ESP_LOGI(TAG, "asset benchmark: mode=preloaded frames=%u elapsed=%lldus avg_frame=%lldus fps=%.2f", count, static_cast<long long>(elapsed), static_cast<long long>(elapsed / count), count * 1000000.0 / elapsed);
}

void run_color_benchmark(lv_display_t* d, lv_obj_t* screen) {
    constexpr std::uint32_t count = 60;
    const auto start = esp_timer_get_time();
    for (std::uint32_t i = 0; i < count; ++i) {
        lv_obj_set_style_bg_color(screen, lv_color_hex((i & 1) ? 0x0000FF : 0xFF0000), 0);
        lv_refr_now(d);
    }
    const auto elapsed = esp_timer_get_time() - start;
    ESP_LOGI(TAG, "color benchmark: mode=full_frame frames=%u elapsed=%lldus avg_frame=%lldus fps=%.2f", count, static_cast<long long>(elapsed), static_cast<long long>(elapsed / count), count * 1000000.0 / elapsed);
}
}

extern "C" void app_main() {
    ESP_LOGI(TAG, "Starting JC1060 LVGL DMA2D asset/color benchmark");
    assets = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "assets");
    constexpr auto caps = MALLOC_CAP_SPIRAM | MALLOC_CAP_DMA | MALLOC_CAP_8BIT;
    image_joy_tears = static_cast<std::uint8_t*>(heap_caps_malloc(IMAGE_BYTES, caps));
    image_sweat_smile = static_cast<std::uint8_t*>(heap_caps_malloc(IMAGE_BYTES, caps));
    auto* first = heap_caps_malloc(FRAME_BYTES, caps);
    auto* second = heap_caps_malloc(FRAME_BYTES, caps);
    if (!assets || !image_joy_tears || !image_sweat_smile || !first || !second || !display.begin()) { ESP_LOGE(TAG, "initialization failed"); return; }
    const auto capabilities = display.capabilities();
    if (!capabilities.dma || !capabilities.vsync || !capabilities.scanout_buffers || capabilities.max_buffer_count < 2) {
        ESP_LOGE(TAG, "DMA scanout capability check failed: dma=%d vsync=%d scanout=%d buffers=%u",
                 capabilities.dma, capabilities.vsync, capabilities.scanout_buffers, capabilities.max_buffer_count);
        return;
    }
    ESP_LOGI(TAG, "DMA scanout ready: dma=%d dma_alignment=%u buffers=%u vsync=%d dma2d=1",
             capabilities.dma, static_cast<unsigned>(capabilities.dma_alignment_bytes), capabilities.max_buffer_count, capabilities.vsync);
    if (!load(generated_assets::Id::joy_tears, image_joy_tears) || !load(generated_assets::Id::sweat_smile, image_sweat_smile)) { ESP_LOGE(TAG, "asset preload failed"); return; }
    gpio_set_direction(GPIO_NUM_23, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_23, 1);
    image_dsc = {.header = {.cf = LV_COLOR_FORMAT_RGB565, .w = 100, .h = 100, .stride = 200}, .data_size = IMAGE_BYTES, .data = image_joy_tears};
    lv_init();
    brick::platform::esp32::LvglDisplayAdapter adapter(display);
    auto* d = adapter.create(LV_DISPLAY_RENDER_MODE_FULL, first, second, FRAME_BYTES);
    if (!d) { ESP_LOGE(TAG, "LVGL initialization failed"); return; }
    auto* screen = lv_screen_active();
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x202040), 0);
    image_widget = lv_image_create(screen); lv_image_set_src(image_widget, &image_dsc); lv_obj_align(image_widget, LV_ALIGN_CENTER, 0, 0);
    lv_refr_now(d);
    vTaskDelay(pdMS_TO_TICKS(1000));
    run_asset_benchmark(d);
    vTaskDelay(pdMS_TO_TICKS(500));
    run_color_benchmark(d, screen);
    ESP_LOGI(TAG, "benchmark complete; repeating every 2 seconds");
    while (true) { vTaskDelay(pdMS_TO_TICKS(2000)); run_asset_benchmark(d); run_color_benchmark(d, screen); }
}
