#include "SdmmcTest.h"

#include <vector>

#include "brick/core/storage/StorageWriteVerify.h"
#include "generated/generated_font.h"
#include "generated/generated_font_large.h"

namespace
{
constexpr char kTag[] = "brick_jc1060_sdmmc";
constexpr char kMountPoint[] = "/sdcard";
constexpr char kTestPath[] = "/sdcard/BRICKTST.BIN";
constexpr std::uint8_t kPattern[] = "BRICK JC1060 SDMMC TEST 2026";
constexpr std::int32_t kStatusStripHeight = 32;
}

SdmmcTest::SdmmcTest(const Services& services) : services_(services), screen_(services.display), touch_timer_(services.time)
{
}

void SdmmcTest::show_status(std::uint16_t color, const char* message)
{
    brick::core::display::Screen::Canvas message_canvas =
        screen_.create_canvas(brick::interfaces::display::DisplayRect{0, 0, 1024, kStatusStripHeight});
    if (!message_canvas.valid())
    {
        services_.logger.error(kTag, "Status canvas allocation failed");
        return;
    }
    message_canvas.clear(0x0000).text(16, 2, message, brick_roboto_20_chars, brick_roboto_20_glyphs, brick_roboto_20_count, color);
    if (!message_canvas.present())
        services_.logger.error(kTag, "Status canvas present failed");

    brick::core::display::Screen::Canvas font_canvas =
        screen_.create_canvas(brick::interfaces::display::DisplayRect{0, kStatusStripHeight, 1024, kStatusStripHeight});
    if (!font_canvas.valid())
    {
        services_.logger.error(kTag, "Font canvas allocation failed");
        return;
    }
    font_canvas.clear(0x0000).text(16, 2, "FONT 28 PX", brick_roboto_28_chars, brick_roboto_28_glyphs, brick_roboto_28_count, color);
    if (!font_canvas.present())
        services_.logger.error(kTag, "Font canvas present failed");
}

bool SdmmcTest::write_read_verify()
{
    return brick::core::storage::write_verify(services_.sd, kTestPath, kPattern, sizeof(kPattern) - 1U);
}

bool SdmmcTest::initialize()
{
    services_.logger.info(kTag, "Starting JC1060 7-inch SDMMC filesystem test");
    show_status(0x001F, "SDMMC INIT");
    if (!services_.sd.mounted() && !services_.sd.mount())
    {
        services_.logger.error(kTag, "SDMMC mount failed");
        show_status(0xF800, "SDMMC MOUNT FAIL");
        touch_timer_.start(1000);
        return true;
    }
    const std::vector<std::string> files = services_.sd.list_files(kMountPoint);
    services_.logger.info(kTag, "Root files: %u", static_cast<unsigned>(files.size()));
    const bool verified = write_read_verify();
    services_.logger.info(kTag, "%s", verified ? "WRITE READ OK" : "WRITE READ FAIL");
    show_status(verified ? 0x07E0 : 0xF800, verified ? "SDMMC READY" : "WRITE READ FAIL");
    touch_timer_.start(1000);
    return true;
}

void SdmmcTest::update()
{
    if (touch_timer_.expired())
    {
        touch_timer_.restart();
        if (services_.sd.mounted() && !services_.sd.probe(kTestPath))
        {
            services_.sd.unmount();
            show_status(0xF800, "SDMMC REMOVED");
        }
        else if (!services_.sd.mounted() && services_.sd.mount())
        {
            const std::vector<std::string> files = services_.sd.list_files(kMountPoint);
            services_.logger.info(kTag, "Root files: %u", static_cast<unsigned>(files.size()));
            show_status(0x07E0, "SDMMC INSERTED");
        }
    }

    std::size_t count = 0;
    const bool touch_down = services_.touch.read(points_.data(), points_.size(), count) && count > 0;
    if (touch_down && !touch_was_down_)
    {
        const bool verified = write_read_verify();
        show_status(verified ? 0x07E0 : 0xF800, verified ? "WRITE READ OK" : "WRITE READ FAIL");
    }
    touch_was_down_ = touch_down;
    services_.time.delay_ms(30);
}
