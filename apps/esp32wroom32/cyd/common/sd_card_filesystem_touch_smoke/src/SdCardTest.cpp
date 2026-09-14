#include "SdCardTest.h"

#include <vector>

#include "brick/core/storage/StorageWriteVerify.h"
#include "generated/generated_font.h"
#include "generated/generated_font_large.h"

namespace cyd_sd_card_filesystem_touch_smoke
{
namespace
{
    constexpr char         kTag[]        = "brick_cyd_sd";
    constexpr char         kMountPoint[] = "/sdcard";
    constexpr char         kTestPath[]   = "/sdcard/BRICKTST.BIN";
    constexpr std::uint8_t kPattern[]    = "BRICK CYD SD TEST 2026";
    constexpr std::int32_t kStatusHeight = 72;
}  // namespace

SdCardTest::SdCardTest(Board& board) : board_(board), screen_(board.display()), card_timer_(board.time())
{
}

void SdCardTest::clear_screen()
{
    brick::core::display::Screen::Canvas canvas =
        screen_.create_canvas(brick::interfaces::display::DisplayRect{ 0, 0, 320, 240 });
    if (canvas.valid())
        canvas.clear(0x0000).present();
}

void SdCardTest::show_status(std::uint16_t color, const char* message)
{
    brick::core::display::Screen::Canvas canvas =
        screen_.create_canvas(brick::interfaces::display::DisplayRect{ 0, 0, 320, kStatusHeight });
    if (!canvas.valid())
        return;
    canvas.clear(0x0000)
        .text(6, 5, message, brick_roboto_16_chars, brick_roboto_16_glyphs, brick_roboto_16_count, color)
        .text(6, 31, "FONT 22 PX", brick_roboto_22_chars, brick_roboto_22_glyphs, brick_roboto_22_count, color)
        .present();
}

bool SdCardTest::write_read_verify()
{
    return brick::core::storage::write_verify(board_.sd(), kTestPath, kPattern, sizeof(kPattern) - 1U);
}

void SdCardTest::refresh_card_status()
{
    if (board_.sd().mounted() && !board_.sd().probe(kTestPath))
    {
        board_.sd().unmount();
        show_status(0xF800, "SD REMOVED");
        return;
    }
    if (!board_.sd().mounted() && board_.sd().mount())
    {
        const std::vector<std::string> files = board_.sd().list_files(kMountPoint);
        board_.logger().info(kTag, "Root files: %u", static_cast<unsigned>(files.size()));
        show_status(0x07E0, "SD INSERTED");
    }
}

bool SdCardTest::initialize()
{
    board_.logger().info(kTag, "CYD SD card filesystem and font smoke");
    clear_screen();
    show_status(0x001F, "SD INIT");
    refresh_card_status();
    if (board_.sd().mounted())
    {
        const bool verified = write_read_verify();
        show_status(verified ? 0x07E0 : 0xF800, verified ? "SD READY" : "WRITE READ FAIL");
    }
    else
        show_status(0xF800, "INSERT SD CARD");
    card_timer_.start(1000);
    return true;
}

void SdCardTest::update()
{
    if (card_timer_.expired())
    {
        card_timer_.restart();
        refresh_card_status();
    }

    std::size_t count      = 0;
    const bool  touch_down = board_.touch().read(points_.data(), points_.size(), count) && count > 0;
    if (touch_down && !touch_was_down_)
    {
        const bool verified = board_.sd().mounted() && write_read_verify();
        show_status(verified ? 0x07E0 : 0xF800, verified ? "WRITE READ OK" : "INSERT SD CARD");
    }
    touch_was_down_ = touch_down;
    board_.time().delay_ms(30);
}
}  // namespace cyd_sd_card_filesystem_touch_smoke
