#include <cstddef>
#include <cstdint>

#include "brick/platform/esp32/LvglDisplayAdapter.h"
#include "brick/core/image/AssetStreamer.h"
#ifdef BRICK_LVGL_TOUCH
#include "brick/platform/esp32/LvglTouchAdapter.h"
#include "brick/boards/esp32/p4/Jc8012Board.h"
#endif
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
constexpr char kTag[] = "brick_jc8012_lvgl";
#ifndef BRICK_PANEL_ROTATION
#define BRICK_PANEL_ROTATION 0
#endif

#if BRICK_PANEL_ROTATION == 0
constexpr auto kRotation = brick::interfaces::display::Rotation::rotate_0;
constexpr std::uint16_t kWidth = 800, kHeight = 1280;
#elif BRICK_PANEL_ROTATION == 90
constexpr auto kRotation = brick::interfaces::display::Rotation::rotate_90;
constexpr std::uint16_t kWidth = 1280, kHeight = 800;
#elif BRICK_PANEL_ROTATION == 180
constexpr auto kRotation = brick::interfaces::display::Rotation::rotate_180;
constexpr std::uint16_t kWidth = 800, kHeight = 1280;
#elif BRICK_PANEL_ROTATION == 270
constexpr auto kRotation = brick::interfaces::display::Rotation::rotate_270;
constexpr std::uint16_t kWidth = 1280, kHeight = 800;
#else
#error "BRICK_PANEL_ROTATION must be 0, 90, 180 or 270"
#endif

constexpr std::size_t kFrameBytes = static_cast<std::size_t>(kWidth) * kHeight * 2;
constexpr std::size_t kImageBytes = 100 * 100 * 2;
brick::platform::esp32::p4::Jc8012Board board(kRotation);
auto& display = board.display();
#ifdef BRICK_LVGL_TOUCH
auto& touch = board.touch();
#endif
const esp_partition_t* assets_partition = nullptr;
std::uint8_t* image_pixels[2] = {};
std::uint8_t* stream_scratch = nullptr;
lv_obj_t* image_widget = nullptr;
lv_obj_t* status_label = nullptr;
lv_image_dsc_t image_descriptors[2]{};
bool showing_joy_tears = true;
std::uint32_t changes = 0;

#ifdef BRICK_LVGL_TOUCH
void set_asset(generated_assets::Id id);

void touch_asset_event(lv_event_t*) {
    showing_joy_tears = !showing_joy_tears;
    ++changes;
    set_asset(showing_joy_tears ? generated_assets::Id::joy_tears
                                : generated_assets::Id::sweat_smile);
    ESP_LOGI(kTag, "touch asset=%s changes=%u",
             showing_joy_tears ? "joy_tears" : "sweat_smile",
             static_cast<unsigned>(changes));
}
#endif

#ifdef BRICK_STREAM_TEST
class PartitionAssetSource final : public brick::interfaces::display::IAssetSource {
public:
    explicit PartitionAssetSource(const esp_partition_t* partition) : partition_(partition) {}

    bool read(const brick::interfaces::display::AssetDescriptor& asset,
              std::size_t offset, std::uint8_t* destination, std::size_t bytes) override {
        return partition_ != nullptr && destination != nullptr &&
               offset <= asset.size && bytes <= asset.size - offset &&
               esp_partition_read(partition_, asset.offset + offset, destination, bytes) == ESP_OK;
    }

private:
    const esp_partition_t* partition_ = nullptr;
};

PartitionAssetSource* asset_source = nullptr;
brick::core::image::AssetStreamer* asset_streamer = nullptr;
#endif

bool load_asset(generated_assets::Id id, std::uint8_t* target) {
    const auto* asset = generated_assets::find(id);
    return assets_partition && target && asset && asset->size == kImageBytes &&
           esp_partition_read(assets_partition, asset->offset, target,
                              asset->size) == ESP_OK;
}

void set_asset(generated_assets::Id id) {
    const auto index = id == generated_assets::Id::joy_tears ? 0U : 1U;
    lv_image_set_src(image_widget, &image_descriptors[index]);
#ifndef BRICK_IMAGE_ONLY
    lv_label_set_text_fmt(status_label, "Asset z flash | zmiany: %u",
                          static_cast<unsigned>(changes));
#endif
}

#ifdef BRICK_STREAM_TEST
bool prefetch_asset(generated_assets::Id id) {
    const auto index = id == generated_assets::Id::joy_tears ? 0U : 1U;
    const auto* descriptor = generated_assets::find(id);
    if (descriptor == nullptr || asset_source == nullptr || asset_streamer == nullptr ||
        !asset_streamer->stream_to_buffer(
            *descriptor, *asset_source,
            {image_pixels[index], descriptor->width, descriptor->height,
             descriptor->stride_bytes, descriptor->format, true},
            stream_scratch, kImageBytes / 2)) {
        ESP_LOGE(kTag, "stream asset load failed: id=%u", static_cast<unsigned>(id));
        return false;
    }
    return true;
}
#endif
}  // namespace

extern "C" void app_main() {
    ESP_LOGI(kTag, "Starting JC8012 10-inch LVGL full-frame asset test: %ux%u rotation=%d frame_bytes=%u",
             kWidth, kHeight, BRICK_PANEL_ROTATION, static_cast<unsigned>(kFrameBytes));
    assets_partition = esp_partition_find_first(
        ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "assets");
    constexpr auto kDmaPsramCaps = MALLOC_CAP_SPIRAM | MALLOC_CAP_DMA | MALLOC_CAP_8BIT;
    image_pixels[0] = static_cast<std::uint8_t*>(heap_caps_malloc(kImageBytes, kDmaPsramCaps));
    image_pixels[1] = static_cast<std::uint8_t*>(heap_caps_malloc(kImageBytes, kDmaPsramCaps));
#ifdef BRICK_STREAM_TEST
    stream_scratch = static_cast<std::uint8_t*>(heap_caps_malloc(kImageBytes / 2, kDmaPsramCaps));
#endif
    auto* first = heap_caps_malloc(kFrameBytes, kDmaPsramCaps);
    if (!assets_partition || !image_pixels[0] || !image_pixels[1] || !first
#ifdef BRICK_STREAM_TEST
        || !stream_scratch
#endif
    ) {
        ESP_LOGE(kTag, "asset partition or PSRAM allocation failed");
        return;
    }
    gpio_set_direction(GPIO_NUM_23, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_23, 0);
    if (!load_asset(generated_assets::Id::joy_tears, image_pixels[0])
#ifndef BRICK_STREAM_TEST
        || !load_asset(generated_assets::Id::sweat_smile, image_pixels[1])
#endif
        || !board.begin()
#ifdef BRICK_LVGL_TOUCH
#endif
    ) {
        ESP_LOGE(kTag, "display or asset initialization failed");
        return;
    }
    const auto capabilities = display.capabilities();
    if (!capabilities.dma || !capabilities.vsync || !capabilities.scanout_buffers || capabilities.max_buffer_count < 2) {
        ESP_LOGE(kTag, "DMA scanout capability check failed: dma=%d vsync=%d scanout=%d buffers=%u",
                 capabilities.dma, capabilities.vsync, capabilities.scanout_buffers, capabilities.max_buffer_count);
        return;
    }
    ESP_LOGI(kTag, "DMA scanout ready: dma=%d dma_alignment=%u buffers=%u vsync=%d dma2d=1",
             capabilities.dma, static_cast<unsigned>(capabilities.dma_alignment_bytes),
             capabilities.max_buffer_count, capabilities.vsync);
    gpio_set_level(GPIO_NUM_23, 1);
#ifdef BRICK_STREAM_TEST
    PartitionAssetSource source(assets_partition);
    brick::core::image::AssetStreamer streamer(display);
    asset_source = &source;
    asset_streamer = &streamer;
#endif
    lv_init();
    brick::platform::esp32::LvglDisplayAdapter adapter(display);
    if (!adapter.create(LV_DISPLAY_RENDER_MODE_FULL, first, nullptr,
                        static_cast<std::uint32_t>(kFrameBytes))) {
        ESP_LOGE(kTag, "LVGL display creation failed");
        return;
    }
#ifdef BRICK_LVGL_TOUCH
    brick::platform::esp32::LvglTouchAdapter touch_adapter(touch);
    if (touch_adapter.create() == nullptr) {
        ESP_LOGE(kTag, "LVGL touch input creation failed");
        return;
    }
#endif
    for (auto index = 0U; index < 2U; ++index)
        image_descriptors[index] = {.header = {.cf = LV_COLOR_FORMAT_RGB565, .w = 100, .h = 100, .stride = 200},
                                    .data_size = kImageBytes, .data = image_pixels[index]};
    auto* screen = lv_screen_active();
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x202040), 0);
#ifndef BRICK_COLOR_ONLY
#ifndef BRICK_IMAGE_ONLY
    auto* title = lv_label_create(screen);
    lv_label_set_text(title, "JC8012 10 LVGL ASSET");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 40);
#endif
    image_widget = lv_image_create(screen);
    lv_image_set_src(image_widget, &image_descriptors[0]);
    lv_obj_align(image_widget, LV_ALIGN_CENTER, 0, -40);
#ifndef BRICK_IMAGE_ONLY
    status_label = lv_label_create(screen);
    lv_label_set_text(status_label, "Asset z flash | zmiany: 0");
    lv_obj_align(status_label, LV_ALIGN_BOTTOM_MID, 0, -40);
#ifdef BRICK_LVGL_TOUCH
    auto* button = lv_button_create(screen);
    lv_obj_align(button, LV_ALIGN_BOTTOM_MID, 0, -100);
    lv_obj_add_event_cb(button, touch_asset_event, LV_EVENT_CLICKED, nullptr);
    auto* button_label = lv_label_create(button);
    lv_label_set_text(button_label, "Przelacz asset");
    lv_obj_center(button_label);
#endif
#endif
#else
    lv_obj_set_style_bg_color(screen, lv_color_hex(0xE00000), 0);
#endif
    ESP_LOGI(kTag, "LVGL initialized: mode=FULL framebuffer_bytes=%u asset_bytes=%u",
             static_cast<unsigned>(kFrameBytes), static_cast<unsigned>(kImageBytes));
    lv_refr_now(lv_display_get_default());
#ifdef BRICK_FPS_TEST
    std::uint32_t fps_frames = 0;
    auto fps_start = esp_timer_get_time();
    while (true) {
        showing_joy_tears = !showing_joy_tears;
#ifdef BRICK_COLOR_ONLY
        lv_obj_set_style_bg_color(screen, showing_joy_tears ? lv_color_hex(0xE00000) : lv_color_hex(0x0000E0), 0);
#else
        set_asset(showing_joy_tears ? generated_assets::Id::joy_tears
                                    : generated_assets::Id::sweat_smile);
#endif
        lv_tick_inc(1);
        lv_timer_handler();
        lv_refr_now(lv_display_get_default());
        ++fps_frames;
        if (fps_frames == 60) {
            const auto elapsed = esp_timer_get_time() - fps_start;
            ESP_LOGI(kTag, "fps benchmark: mode=%s rotation=%d frames=%u elapsed=%lldus fps=%.2f",
#ifdef BRICK_COLOR_ONLY
                     "color",
#else
                     "asset",
#endif
                     BRICK_PANEL_ROTATION, static_cast<unsigned>(fps_frames), static_cast<long long>(elapsed),
                     static_cast<double>(fps_frames) * 1000000.0 / static_cast<double>(elapsed));
            fps_frames = 0;
            fps_start = esp_timer_get_time();
        }
        vTaskDelay(pdMS_TO_TICKS(1));
    }
#else
    std::uint32_t tick = 0;
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(10));
        lv_tick_inc(10);
        lv_timer_handler();
#ifdef BRICK_STREAM_TEST
        if (tick == 49) {
            const auto next = showing_joy_tears ? generated_assets::Id::sweat_smile
                                                 : generated_assets::Id::joy_tears;
            if (prefetch_asset(next))
                ESP_LOGI(kTag, "prefetch=%s", showing_joy_tears ? "sweat_smile" : "joy_tears");
        }
#endif
        if (++tick >= 100) {
            tick = 0;
#ifndef BRICK_LVGL_TOUCH
#ifdef BRICK_COLOR_ONLY
            showing_joy_tears = !showing_joy_tears;
            ++changes;
            lv_obj_set_style_bg_color(screen, showing_joy_tears ? lv_color_hex(0xE00000) : lv_color_hex(0x0000E0), 0);
            ESP_LOGI(kTag, "color=%s changes=%u", showing_joy_tears ? "red" : "blue",
                     static_cast<unsigned>(changes));
#else
            showing_joy_tears = !showing_joy_tears;
            ++changes;
            set_asset(showing_joy_tears ? generated_assets::Id::joy_tears
                                        : generated_assets::Id::sweat_smile);
            ESP_LOGI(kTag, "asset=%s changes=%u",
                     showing_joy_tears ? "joy_tears" : "sweat_smile",
                     static_cast<unsigned>(changes));
#endif
#endif
        }
    }
#endif
}
