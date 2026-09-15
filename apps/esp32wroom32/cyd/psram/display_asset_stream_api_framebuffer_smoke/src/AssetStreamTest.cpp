#include "AssetStreamTest.h"

namespace cyd_asset_stream_smoke
{
namespace
{
    constexpr char kTag[] = "brick_cyd_asset";
}  // namespace

AssetStreamTest::AssetStreamTest(brick::interfaces::display::IDisplayDevice& display,
                                 brick::interfaces::storage::IFileSystem& filesystem,
                                 brick::interfaces::display::ITouchscreen& touch,
                                 brick::interfaces::time::ITimeProvider& time,
                                 brick::interfaces::logging::ILogger& logger)
    : time_(time), logger_(logger), sources_(filesystem), presenter_(display, buffers_),
      playback_(sources_, buffers_, presenter_, display, time, logger), touch_(touch)
{
}

bool AssetStreamTest::initialize()
{
    logger_.info(kTag, "CYD asset benchmark: flash, PSRAM and SD");
    if (!buffers_.initialize() || !sources_.initialize() || !playback_.initialize())
    {
        logger_.error(kTag, "PSRAM buffers or assets partition unavailable");
        return false;
    }
    return true;
}

void AssetStreamTest::update()
{
    if (touch_.pressed())
    {
        if (!playback_.advance_mode())
            logger_.warning(kTag, "Requested storage unavailable; using flash");
        logger_.info(kTag, "touch: storage=%s mode=%s", playback_.storage_name(),
                             playback_.backgrounds() ? "red/blue" : "smiles");
    }
    if (!playback_.present_next())
        logger_.error(kTag, "asset presentation failed");
    time_.delay_ms(1);
}
}  // namespace cyd_asset_stream_smoke
