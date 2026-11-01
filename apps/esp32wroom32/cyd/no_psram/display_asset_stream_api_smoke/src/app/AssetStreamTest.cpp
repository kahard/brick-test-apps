#include "AssetStreamTest.h"

#include "generated_assets.h"

namespace cyd_asset_stream_smoke
{
namespace
{
    constexpr char          kTag[]        = "cyd_asset_stream";
    constexpr std::uint16_t kWidth        = 320U;
    constexpr std::uint16_t kHeight       = 240U;
    constexpr std::uint16_t kStripeHeight = 16U;
    constexpr std::size_t   kStripeBytes  = kWidth * kStripeHeight * 2U;
    constexpr std::uint32_t kRefreshTime  = 1000U;
}  // namespace

AssetStreamTest::AssetStreamTest(const Services& services)
    : time_(services.time), logger_(services.logger), assets_(services.assets), stripe_(services.memory),
      streamer_(services.display), refresh_timer_(services.time), touch_(services.touch)
{
}

Status AssetStreamTest::initialize()
{
    logger_.info(kTag, "CYD asset stream: flash partition to internal DMA stripe buffer");
    if (!is_ok(stripe_.allocate(kStripeBytes, brick::interfaces::memory::MemoryKind::dma)))
    {
        logger_.error(kTag, "Internal DMA stripe buffer unavailable");
        return Status::error;
    }
    refresh_timer_.start(kRefreshTime);
    return show_selected_asset();
}

void AssetStreamTest::update()
{
    if (touch_.pressed())
    {
        select_next_asset();
        report(show_selected_asset());
    }
    else if (refresh_timer_.expired())
        report(show_selected_asset());
}

Status AssetStreamTest::show_selected_asset()
{
    const generated_assets::Id id =
        selected_ == 0U ? generated_assets::Id::joy_tears : generated_assets::Id::sweat_smile;
    const brick::interfaces::display::AssetDescriptor* asset = generated_assets::get(id);
    if (asset == nullptr)
        return Status::error;

    const std::uint64_t started_us = time_.micros();
    const Status status = streamer_.stream(*asset, assets_, { 0, 0, kWidth, kHeight }, stripe_.data(), stripe_.size());
    if (is_ok(status))
    {
        const char* name = selected_ == 0U ? "joy_tears" : "sweat_smile";
        logger_.info(kTag, "asset=%s streamed=%llu us", name,
                     static_cast<unsigned long long>(time_.micros() - started_us));
    }
    refresh_timer_.restart();
    return status;
}

void AssetStreamTest::report(Status status)
{
    if (!is_ok(status))
        logger_.error(kTag, "Asset stream failed: %s", to_string(status));
}

void AssetStreamTest::select_next_asset()
{
    selected_ ^= 1U;
}
}  // namespace cyd_asset_stream_smoke
