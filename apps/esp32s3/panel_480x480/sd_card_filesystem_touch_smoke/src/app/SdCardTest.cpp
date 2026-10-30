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

SdCardTest::SdCardTest(const Services& services) : services_(services), screen_(services.display), card_timer_(services.time)
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
    return brick::core::storage::write_verify(services_.sd, kTestPath, kPattern, sizeof(kPattern) - 1U);
}

bool SdCardTest::initialize()
{
    services_.logger.info(kTag, "ESP32-S3 4\" SD card filesystem smoke");
    show_status(0x001F, "SD INIT");
    if (!services_.sd.mounted() && !services_.sd.mount())
    {
        show_status(0xF800, "SD MOUNT FAIL");
        return false;
    }
    const std::vector<std::string> files = services_.sd.list_files(kMountPoint);
    services_.logger.info(kTag, "Root files: %u", static_cast<unsigned>(files.size()));
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
        if (services_.sd.mounted() && !services_.sd.probe(kTestPath))
        {
            services_.sd.unmount();
            show_status(0xF800, "SD REMOVED");
        }
        else if (!services_.sd.mounted() && services_.sd.mount())
            show_status(0x07E0, "SD INSERTED");
    }

    std::size_t count      = 0;
    const bool  touch_down = services_.touch.read(points_.data(), points_.size(), count) && count > 0;
    if (touch_down && !touch_was_down_)
    {
        const bool verified = write_read_verify();
        show_status(verified ? 0x07E0 : 0xF800, verified ? "WRITE READ OK" : "WRITE READ FAIL");
    }
    touch_was_down_ = touch_down;
    services_.time.delay_ms(30);
}
