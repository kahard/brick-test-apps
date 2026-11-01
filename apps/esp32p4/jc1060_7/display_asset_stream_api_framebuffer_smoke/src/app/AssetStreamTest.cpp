#include "AssetStreamTest.h"

namespace asset_stream_smoke
{
AssetStreamTest::AssetStreamTest(AssetPlayback& playback, Touchscreen& touch, TimeProvider& time, Logger& logger)
    : playback_(playback), touch_(touch), benchmark_(time, logger), logger_(logger)
{
}

Status AssetStreamTest::initialize()
{
    if (!is_ok(playback_.initialize()))
        return Status::error;
    log_mode();
    benchmark_.reset();
    return Status::ok;
}

void AssetStreamTest::log_mode()
{
    const char* storage = playback_.storage() == 0 ? "flash" : playback_.storage() == 1 ? "psram" : "sdmmc";
    logger_.info("assets", "storage=%s mode=%s (two render buffers -> MIPI scan-out)", storage,
                 playback_.backgrounds() ? "R/B" : "smiles");
}

void AssetStreamTest::update()
{
    if (failed_)
        return;
    if (touch_.pressed())
    {
        if (!is_ok(playback_.advance_mode()))
            logger_.warning("assets", "Requested source unavailable; falling back to flash");
        log_mode();
        benchmark_.reset();
    }
    const std::uint8_t previous_storage = playback_.storage();
    if (!is_ok(playback_.present_next()))
    {
        failed_ = true;
        logger_.error("assets", "Asset read or display transfer failed");
        return;
    }
    if (previous_storage != playback_.storage())
    {
        logger_.warning("assets", "SD read failed; falling back to flash");
        log_mode();
        benchmark_.reset();
    }
    benchmark_.frame();
}
}  // namespace asset_stream_smoke
