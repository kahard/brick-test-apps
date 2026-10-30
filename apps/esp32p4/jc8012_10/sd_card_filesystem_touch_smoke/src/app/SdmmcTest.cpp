#include "SdmmcTest.h"

#include "brick/core/storage/StorageWriteVerify.h"
#include "generated/generated_font.h"
#include "generated/generated_font_large.h"

namespace
{
constexpr char         kTag[]             = "brick_jc8012_sdmmc";
constexpr char         kMountPoint[]      = "/sdcard";
constexpr char         kTestPath[]        = "/sdcard/BRICKTST.BIN";
constexpr std::uint8_t kPattern[]         = "BRICK JC8012 SDMMC TEST 2026";
constexpr std::int32_t kStatusStripHeight = 32;
}  // namespace

SdmmcTest::SdmmcTest(const Services& services)
    : services_(services), screen_(services.display), touch_timer_(services.time),
      touch_diagnostic_timer_(services.time)
{
}

void SdmmcTest::show_status(std::uint16_t color, const char* message)
{
    brick::core::display::Screen::Canvas message_canvas =
        screen_.create_canvas(brick::interfaces::display::DisplayRect{ 0, 0, 800, kStatusStripHeight }, canvas_pixels_);
    if (!message_canvas.valid())
    {
        services_.logger.error(kTag, "Status canvas does not fit its buffer");
        return;
    }
    message_canvas.clear(0x0000).text(16, 2, message, brick_roboto_20_chars, brick_roboto_20_glyphs,
                                      brick_roboto_20_count, color);
    if (!message_canvas.present())
        services_.logger.error(kTag, "Status canvas present failed");

    brick::core::display::Screen::Canvas font_canvas = screen_.create_canvas(
        brick::interfaces::display::DisplayRect{ 0, kStatusStripHeight, 800, kStatusStripHeight }, canvas_pixels_);
    if (!font_canvas.valid())
    {
        services_.logger.error(kTag, "Font canvas does not fit its buffer");
        return;
    }
    font_canvas.clear(0x0000).text(16, 2, "FONT 28 PX", brick_roboto_28_chars, brick_roboto_28_glyphs,
                                   brick_roboto_28_count, color);
    if (!font_canvas.present())
        services_.logger.error(kTag, "Font canvas present failed");
}

bool SdmmcTest::write_read_verify()
{
    return brick::core::storage::write_verify(services_.sd, kTestPath, kPattern, sizeof(kPattern) - 1U);
}

bool SdmmcTest::initialize()
{
    services_.logger.info(kTag, "Starting JC8012 10-inch SDMMC filesystem test");
    show_status(0x001F, "SDMMC INIT");
    if (!services_.sd.mounted() && !services_.sd.mount())
    {
        services_.logger.error(kTag, "SDMMC mount failed");
        show_status(0xF800, "SDMMC MOUNT FAIL");
        touch_timer_.start(1000);
        touch_diagnostic_timer_.start(1000);
        return true;
    }
    const std::size_t files = services_.sd.list_files(kMountPoint, nullptr, 0);
    services_.logger.info(kTag, "Root files: %u", static_cast<unsigned>(files));
    const bool verified = write_read_verify();
    services_.logger.info(kTag, "%s", verified ? "WRITE READ OK" : "WRITE READ FAIL");
    show_status(verified ? 0x07E0 : 0xF800, verified ? "SDMMC READY" : "WRITE READ FAIL");
    touch_timer_.start(1000);
    touch_diagnostic_timer_.start(1000);
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
            const std::size_t files = services_.sd.list_files(kMountPoint, nullptr, 0);
            services_.logger.info(kTag, "Root files: %u", static_cast<unsigned>(files));
            show_status(0x07E0, "SDMMC INSERTED");
        }
    }

    std::size_t count      = 0;
    const bool  touch_read = services_.touch.read(points_.data(), points_.size(), count);
    const bool  touch_down = touch_read && count > 0;
    if (!touch_read && touch_diagnostic_timer_.expired())
    {
        touch_diagnostic_timer_.restart();
        services_.logger.warning(kTag, "Touch controller read failed");
    }
    if (touch_down && !touch_was_down_)
    {
        services_.logger.info(kTag, "Touch: count=%u x=%d y=%d", static_cast<unsigned>(count), points_[0].x,
                              points_[0].y);
        services_.logger.info(kTag, "Touch: write/read test requested");
        const bool verified = write_read_verify();
        services_.logger.info(kTag, "%s", verified ? "WRITE READ OK" : "WRITE READ FAIL");
        show_status(verified ? 0x07E0 : 0xF800, verified ? "WRITE READ OK" : "WRITE READ FAIL");
    }
    touch_was_down_ = touch_down;
    services_.time.delay_ms(30);
}
