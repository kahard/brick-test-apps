#include <algorithm>
#include <array>

#include "brick/boards/esp32/s3/Panel480Board.h"
#include "brick/core/display/Screen.h"
#include "brick/core/storage/StorageWriteVerify.h"
#include "brick/core/time/Timer.h"
#include "brick/interfaces/display/PixelBuffer.h"
#include "generated/generated_font.h"
#include "generated/generated_font_large.h"

namespace
{
constexpr char kTag[]        = "brick_sd_card_smoke";
constexpr char kMountPoint[] = "/sdcard";
// Keep the filename in FAT 8.3 form; this panel's FAT configuration does not
// accept long filenames.
constexpr char kTestPath[] = "/sdcard/BRICKTST.BIN";

brick::platform::esp32::s3::Panel480Board g_board;
brick::core::display::Screen              g_screen(g_board.display());

void show_status(std::uint16_t color, const char* message)
{
    constexpr std::uint16_t              kStatusHeight = 96;
    constexpr std::uint16_t              kTextTop      = 8;
    brick::core::display::Screen::Canvas canvas        = g_screen.create_canvas(kStatusHeight);
    const bool                           submitted     = canvas.valid();
    if (submitted)
    {
        canvas.clear(0x0000)
            .text(8, kTextTop, message, brick_roboto_20_chars, brick_roboto_20_glyphs, brick_roboto_20_count, color)
            .text(8, 42, "FONT 28 PX", brick_roboto_28_chars, brick_roboto_28_glyphs, brick_roboto_28_count, color);
    }
    const bool completed = submitted && canvas.present();
    g_board.logger().info(kTag, "status '%s': draw=%d complete=%d", message, submitted ? 1 : 0, completed ? 1 : 0);
}

}  // namespace

extern "C" void app_main()
{
    g_board.logger().info(kTag, "ESP32-S3 4\" SD card filesystem smoke");
    if (!g_board.begin())
    {
        g_board.logger().error(kTag, "Board init failed");
        return;
    }
    show_status(0x001F, "SD INIT");
    if (!g_board.sd_card().mount())
    {
        show_status(0xF800, "SD MOUNT FAIL");
        g_board.logger().error(kTag, "SD CARD TEST FAIL: mount");
        return;
    }
    const std::vector<std::string> files = g_board.sd_card().list_files(kMountPoint);
    g_board.logger().info(kTag, "Root files: %u", static_cast<unsigned>(files.size()));
    constexpr std::uint8_t pattern[] = "BRICK SD CARD TEST 2026";
    if (!brick::core::storage::write_verify(g_board.sd_card(), kTestPath, pattern, sizeof(pattern) - 1U))
    {
        show_status(0xF800, "WRITE READ FAIL");
        g_board.logger().error(kTag, "SD CARD TEST FAIL: write/read");
    }
    else
        show_status(0x07E0, "SD READY");
    g_board.logger().info(kTag, "SD CARD TEST PASS");
    std::array<brick::interfaces::display::TouchPoint, 5> points{};
    bool                                                  touch_was_down = false;
    brick::core::time::Timer                              card_timer(g_board.time());
    card_timer.start(1000);
    while (true)
    {
        if (card_timer.expired())
        {
            card_timer.restart();
            if (g_board.sd_card().mounted())
            {
                if (!g_board.sd_card().probe(kTestPath))
                {
                    g_board.sd_card().unmount();
                    show_status(0xF800, "SD REMOVED");
                }
            }
            else if (g_board.sd_card().mount())
            {
                const std::vector<std::string> files = g_board.sd_card().list_files(kMountPoint);
                g_board.logger().info(kTag, "Root files: %u", static_cast<unsigned>(files.size()));
                show_status(0x07E0, "SD INSERTED");
            }
        }
        std::size_t count      = 0;
        const bool  touch_down = g_board.touch().read(points.data(), points.size(), count) && count > 0;
        if (touch_down && !touch_was_down)
        {
            if (brick::core::storage::write_verify(g_board.sd_card(), kTestPath, pattern, sizeof(pattern) - 1U))
                show_status(0x07E0, "WRITE READ OK");
            else
                show_status(0xF800, "WRITE READ FAIL");
        }
        touch_was_down = touch_down;
        g_board.time().delay_ms(30);
    }
}
