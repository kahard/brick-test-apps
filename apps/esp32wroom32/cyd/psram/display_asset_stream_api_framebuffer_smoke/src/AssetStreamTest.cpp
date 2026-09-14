#include "AssetStreamTest.h"

namespace cyd_asset_stream_smoke
{
namespace
{
    constexpr char kTag[] = "brick_cyd_asset";
}  // namespace

AssetStreamTest::AssetStreamTest(Board& board) : board_(board)
{
}

bool AssetStreamTest::initialize()
{
    board_.logger().info(kTag, "CYD asset benchmark: flash, PSRAM and SD");
    if (!buffers_.initialize() || !sources_.initialize() || !playback_.initialize())
    {
        board_.logger().error(kTag, "PSRAM buffers or assets partition unavailable");
        return false;
    }
    return true;
}

void AssetStreamTest::update()
{
    if (touch_.pressed())
    {
        if (!playback_.advance_mode())
            board_.logger().warning(kTag, "Requested storage unavailable; using flash");
        board_.logger().info(kTag, "touch: storage=%s mode=%s", playback_.storage_name(),
                             playback_.backgrounds() ? "red/blue" : "smiles");
    }
    if (!playback_.present_next())
        board_.logger().error(kTag, "asset presentation failed");
    board_.time().delay_ms(1);
}
}  // namespace cyd_asset_stream_smoke
