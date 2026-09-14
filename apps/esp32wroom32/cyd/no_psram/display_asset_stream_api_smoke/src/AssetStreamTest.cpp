#include "AssetStreamTest.h"

#include "generated_assets.h"

namespace cyd_asset_stream_smoke
{
namespace
{
    constexpr char          kTag[]       = "cyd_asset_stream";
    constexpr std::uint16_t kWidth       = 320U;
    constexpr std::uint16_t kHeight      = 240U;
    constexpr std::uint32_t kRefreshTime = 1000U;
}  // namespace

AssetStreamTest::AssetStreamTest(Board& board) : board_(board)
{
}

bool AssetStreamTest::initialize()
{
    board_.logger().info(kTag, "CYD asset stream: flash partition to internal DMA stripe buffer");
    if (!stripe_.initialize() || !assets_.begin())
    {
        board_.logger().error(kTag, "Internal DMA buffer or assets partition unavailable");
        return false;
    }
    refresh_timer_.start(kRefreshTime);
    return show_selected_asset();
}

void AssetStreamTest::update()
{
    if (touch_.pressed())
    {
        select_next_asset();
        show_selected_asset();
    }
    else if (refresh_timer_.expired())
        show_selected_asset();
    board_.time().delay_ms(10U);
}

bool AssetStreamTest::show_selected_asset()
{
    const generated_assets::Id id =
        selected_ == 0U ? generated_assets::Id::joy_tears : generated_assets::Id::sweat_smile;
    const brick::interfaces::display::AssetDescriptor* asset = generated_assets::get(id);
    if (asset == nullptr)
        return false;

    const std::uint64_t started_us = board_.time().micros();
    const bool streamed = streamer_.stream(*asset, assets_, { 0, 0, kWidth, kHeight }, stripe_.data(), stripe_.size());
    if (streamed)
    {
        const char* name = selected_ == 0U ? "joy_tears" : "sweat_smile";
        board_.logger().info(kTag, "asset=%s streamed=%llu us", name,
                             static_cast<unsigned long long>(board_.time().micros() - started_us));
    }
    else
        board_.logger().error(kTag, "Asset stream failed");
    refresh_timer_.restart();
    return streamed;
}

void AssetStreamTest::select_next_asset()
{
    selected_ ^= 1U;
}
}  // namespace cyd_asset_stream_smoke
