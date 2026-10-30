#include "AssetStreamTest.h"

namespace cyd_asset_stream_smoke
{
namespace
{
    constexpr char kTag[] = "brick_cyd_asset";
}  // namespace

AssetStreamTest::AssetStreamTest(const Services& services)
    : logger_(services.logger), sources_(services.flash_assets, services.sd, services.memory),
      buffers_(services.memory), presenter_(services.display, buffers_),
      playback_(sources_, buffers_, presenter_, services.display, services.time, services.logger),
      touch_(services.touch)
{
}

bool AssetStreamTest::initialize()
{
    logger_.info(kTag, "CYD asset benchmark: flash, PSRAM and SD");
    if (!buffers_.initialize() || !playback_.initialize())
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
}
}  // namespace cyd_asset_stream_smoke
