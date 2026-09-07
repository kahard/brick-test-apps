#include "AssetStreamTest.h"

AssetStreamTest::AssetStreamTest(AssetPlayback& playback, brick::interfaces::display::ITouchscreen& touch,
                                 brick::interfaces::time::ITimeProvider& time,
                                 brick::interfaces::logging::ILogger&    logger)
    : playback_(playback), touch_(touch), benchmark_(time, logger), logger_(logger)
{
}

bool AssetStreamTest::initialize()
{
    if (!playback_.initialize())
        return false;
    log_mode();
    benchmark_.reset();
    return true;
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
        if (!playback_.advance_mode())
            logger_.warning("assets", "Requested source unavailable; falling back to flash");
        log_mode();
        benchmark_.reset();
    }
    const std::uint8_t previous_storage = playback_.storage();
    if (!playback_.present_next())
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
