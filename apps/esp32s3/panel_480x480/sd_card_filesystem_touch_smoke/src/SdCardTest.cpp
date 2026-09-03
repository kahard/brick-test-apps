#include "SdCardTest.h"

#include <vector>

#include "brick/core/storage/StorageWriteVerify.h"
#include "generated/generated_font.h"
#include "generated/generated_font_large.h"

namespace
{
constexpr char         kTag[]        = "brick_sd_card_smoke";
constexpr char         kMountPoint[] = "/sdcard";
constexpr char         kTestPath[]   = "/sdcard/BRICKTST.BIN";
constexpr std::uint8_t kPattern[]    = "BRICK SD CARD TEST 2026";
}  // namespace

SdCardTest::SdCardTest(Board& board) : board_(board), screen_(board.display()), card_timer_(board.time())
{
}

void SdCardTest::show_status(std::uint16_t color, const char* message)
{
    brick::core::display::Screen::Canvas canvas =
        screen_.create_canvas(brick::interfaces::display::DisplayRect{ 0, 0, 480, 96 });
    if (!canvas.valid())
        return;
    canvas.clear(0x0000)
        .text(8, 8, message, brick_roboto_20_chars, brick_roboto_20_glyphs, brick_roboto_20_count, color)
        .text(8, 42, "FONT 28 PX", brick_roboto_28_chars, brick_roboto_28_glyphs, brick_roboto_28_count, color)
        .present();
}

bool SdCardTest::write_read_verify()
{
    return brick::core::storage::write_verify(board_.sd_card(), kTestPath, kPattern, sizeof(kPattern) - 1U);
}

bool SdCardTest::initialize()
{
    board_.logger().info(kTag, "ESP32-S3 4\" SD card filesystem smoke");
    show_status(0x001F, "SD INIT");
    if (!board_.sd_card().mounted() && !board_.sd_card().mount())
    {
        show_status(0xF800, "SD MOUNT FAIL");
        return false;
    }
    const std::vector<std::string> files = board_.sd_card().list_files(kMountPoint);
    board_.logger().info(kTag, "Root files: %u", static_cast<unsigned>(files.size()));
    const bool verified = write_read_verify();
    show_status(verified ? 0x07E0 : 0xF800, verified ? "SD READY" : "WRITE READ FAIL");
    card_timer_.start(1000);
    return true;
}

void SdCardTest::update()
{
    if (card_timer_.expired())
    {
        card_timer_.restart();
        if (board_.sd_card().mounted() && !board_.sd_card().probe(kTestPath))
        {
            board_.sd_card().unmount();
            show_status(0xF800, "SD REMOVED");
        }
        else if (!board_.sd_card().mounted() && board_.sd_card().mount())
            show_status(0x07E0, "SD INSERTED");
    }

    std::size_t count      = 0;
    const bool  touch_down = board_.touch().read(points_.data(), points_.size(), count) && count > 0;
    if (touch_down && !touch_was_down_)
    {
        const bool verified = write_read_verify();
        show_status(verified ? 0x07E0 : 0xF800, verified ? "WRITE READ OK" : "WRITE READ FAIL");
    }
    touch_was_down_ = touch_down;
    board_.time().delay_ms(30);
}
