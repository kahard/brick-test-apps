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

Status AssetStreamTest::initialize()
{
    logger_.info(kTag, "CYD asset benchmark: flash, PSRAM and SD");
    if (!is_ok(buffers_.initialize()) || !is_ok(playback_.initialize()))
    {
        logger_.error(kTag, "PSRAM buffers or assets partition unavailable");
        return Status::error;
    }
    return Status::ok;
}

void AssetStreamTest::update()
{
    if (touch_.pressed())
    {
        if (!is_ok(playback_.advance_mode()))
            logger_.warning(kTag, "Requested storage unavailable; using flash");
        logger_.info(kTag, "touch: storage=%s mode=%s", playback_.storage_name(),
                     playback_.backgrounds() ? "red/blue" : "smiles");
    }
    if (!is_ok(playback_.present_next()))
        logger_.error(kTag, "asset presentation failed");
}
}  // namespace cyd_asset_stream_smoke
