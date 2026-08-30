#include <array>
#include <cstdint>

#include "brick/boards/esp32/p4/Jc8012Board.h"
#include "driver/gpio.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace
{
constexpr char TAG[] = "brick_jc8012_touch";
#ifndef BRICK_PANEL_ROTATION
#    define BRICK_PANEL_ROTATION 0
#endif

#if BRICK_PANEL_ROTATION == 0 || BRICK_PANEL_ROTATION == 180
constexpr std::uint16_t kWidth        = 800;
constexpr std::uint16_t kHeight       = 1280;
constexpr std::uint16_t kStripeHeight = 160;
#elif BRICK_PANEL_ROTATION == 90 || BRICK_PANEL_ROTATION == 270
constexpr std::uint16_t kWidth        = 1280;
constexpr std::uint16_t kHeight       = 800;
constexpr std::uint16_t kStripeHeight = 100;
#else
#    error "BRICK_PANEL_ROTATION must be 0, 90, 180 or 270"
#endif

constexpr std::uint16_t                kMarkerSize   = 100;
constexpr std::uint16_t                kMarkerArm    = 20;
constexpr std::uint16_t                kMarkerBorder = 4;
constexpr std::array<std::uint16_t, 8> kStripeColors = {
    0xF800, 0x07E0, 0x001F, 0xFFE0, 0xF81F, 0x07FF, 0x0000, 0xFFFF
};

#if BRICK_PANEL_ROTATION == 0
constexpr auto kRotation = brick::interfaces::display::Rotation::rotate_0;
#elif BRICK_PANEL_ROTATION == 90
constexpr auto kRotation = brick::interfaces::display::Rotation::rotate_90;
#elif BRICK_PANEL_ROTATION == 180
constexpr auto kRotation = brick::interfaces::display::Rotation::rotate_180;
#else
constexpr auto kRotation = brick::interfaces::display::Rotation::rotate_270;
#endif

brick::platform::esp32::p4::Jc8012Board board(kRotation);
auto&                                   display = board.display();
auto&                                   touch   = board.touch();

std::uint16_t background_color(std::uint16_t y)
{
    return kStripeColors[y / kStripeHeight];
}

void fill_background(std::uint16_t* frame)
{
    for (std::uint16_t y = 0; y < kHeight; ++y)
    {
        const auto color = background_color(y);
        for (std::uint16_t x = 0; x < kWidth; ++x)
            frame[static_cast<std::size_t>(y) * kWidth + x] = color;
    }
}

void paint_crosshair(std::uint16_t* frame, const brick::interfaces::display::TouchPoint& point, bool active)
{
    const auto left   = point.x < kMarkerSize / 2 ? 0 : point.x - kMarkerSize / 2;
    const auto top    = point.y < kMarkerSize / 2 ? 0 : point.y - kMarkerSize / 2;
    const auto x0     = left + kMarkerSize > kWidth ? kWidth - kMarkerSize : left;
    const auto y0     = top + kMarkerSize > kHeight ? kHeight - kMarkerSize : top;
    const auto half   = kMarkerSize / 2;
    const auto arm    = kMarkerArm / 2;
    const auto border = kMarkerBorder;
    for (std::uint16_t y = 0; y < kMarkerSize; ++y)
    {
        for (std::uint16_t x = 0; x < kMarkerSize; ++x)
        {
            const bool inner = (x >= half - arm && x < half + arm) || (y >= half - arm && y < half + arm);
            const bool outer = (x >= half - arm - border && x < half + arm + border)
                               || (y >= half - arm - border && y < half + arm + border);
            if (active && outer)
                frame[static_cast<std::size_t>(y0 + y) * kWidth + x0 + x] = inner ? 0x0000 : 0xFFFF;
            else
                frame[static_cast<std::size_t>(y0 + y) * kWidth + x0 + x] = background_color(y0 + y);
        }
    }
}

bool submit_frame(std::uint16_t* frame)
{
    const brick::interfaces::display::PixelBuffer buffer{
        reinterpret_cast<const std::uint8_t*>(frame),    kWidth, kHeight, static_cast<std::size_t>(kWidth) * 2,
        brick::interfaces::display::PixelFormat::rgb565, false
    };
    if (!display.draw_buffer({ 0, 0, kWidth, kHeight }, buffer))
        return false;
    return display.wait_for_transfer_complete(2000);
}
}  // namespace

extern "C" void app_main()
{
    ESP_LOGI(TAG, "Starting JC8012 GSL3680 touch smoke test: logical=%ux%u rotation=%d", kWidth, kHeight,
             BRICK_PANEL_ROTATION);
    gpio_set_direction(GPIO_NUM_23, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_23, 0);
    if (!board.begin())
    {
        ESP_LOGE(TAG, "display or GSL3680 initialization failed");
        return;
    }
    gpio_set_level(GPIO_NUM_23, 1);
    auto* frame = static_cast<std::uint16_t*>(heap_caps_malloc(
        static_cast<std::size_t>(kWidth) * kHeight * sizeof(std::uint16_t), MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
    if (frame == nullptr)
    {
        ESP_LOGE(TAG, "unable to allocate touch test framebuffer");
        return;
    }
    fill_background(frame);
    if (!submit_frame(frame))
        ESP_LOGE(TAG, "initial frame submission failed");
    std::array<brick::interfaces::display::TouchPoint, 5> points{};
    std::array<brick::interfaces::display::TouchPoint, 5> marker_points{};
    std::array<bool, 5>                                   marker_visible{};
    while (true)
    {
        std::size_t count = 0;
        if (touch.read(points.data(), points.size(), count))
        {
            bool frame_dirty = false;
            for (std::size_t i = 0; i < count; ++i)
            {
                const auto& point = points[i];
                if (point.id >= marker_visible.size())
                    continue;
                const bool active = point.state != brick::interfaces::display::TouchState::released;
                const bool moved  = active
                                   && (!marker_visible[point.id] || marker_points[point.id].x != point.x
                                       || marker_points[point.id].y != point.y);
                if (!active)
                {
                    if (marker_visible[point.id])
                    {
                        paint_crosshair(frame, marker_points[point.id], false);
                        frame_dirty = true;
                    }
                    marker_visible[point.id] = false;
                }
                else if (moved)
                {
                    if (marker_visible[point.id])
                    {
                        paint_crosshair(frame, marker_points[point.id], false);
                        frame_dirty = true;
                    }
                    marker_points[point.id]  = point;
                    marker_visible[point.id] = true;
                    frame_dirty              = true;
                }
                if (point.state != brick::interfaces::display::TouchState::moved)
                {
                    ESP_LOGI(TAG, "touch id=%u x=%d y=%d raw=(%d,%d) pressure=%d state=%u", point.id, point.x, point.y,
                             point.raw_x, point.raw_y, point.pressure, static_cast<unsigned>(point.state));
                }
            }
            if (frame_dirty)
            {
                for (std::size_t id = 0; id < marker_visible.size(); ++id)
                    if (marker_visible[id])
                        paint_crosshair(frame, marker_points[id], true);
                if (!submit_frame(frame))
                    ESP_LOGW(TAG, "touch frame submission failed");
            }
        }
        vTaskDelay(pdMS_TO_TICKS(16));
    }
}
