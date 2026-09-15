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

AssetStreamTest::AssetStreamTest(brick::interfaces::display::IDisplayDevice& display,
                                 brick::interfaces::display::ITouchscreen& touch,
                                 brick::interfaces::time::ITimeProvider& time,
                                 brick::interfaces::logging::ILogger& logger)
    : time_(time), logger_(logger), streamer_(display), refresh_timer_(time), touch_(touch)
{
}

bool AssetStreamTest::initialize()
{
    logger_.info(kTag, "CYD asset stream: flash partition to internal DMA stripe buffer");
    if (!stripe_.initialize() || !assets_.begin())
    {
        logger_.error(kTag, "Internal DMA buffer or assets partition unavailable");
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
    time_.delay_ms(10U);
}

bool AssetStreamTest::show_selected_asset()
{
    const generated_assets::Id id =
        selected_ == 0U ? generated_assets::Id::joy_tears : generated_assets::Id::sweat_smile;
    const brick::interfaces::display::AssetDescriptor* asset = generated_assets::get(id);
    if (asset == nullptr)
        return false;

    const std::uint64_t started_us = time_.micros();
    const bool streamed = streamer_.stream(*asset, assets_, { 0, 0, kWidth, kHeight }, stripe_.data(), stripe_.size());
    if (streamed)
    {
        const char* name = selected_ == 0U ? "joy_tears" : "sweat_smile";
        logger_.info(kTag, "asset=%s streamed=%llu us", name,
                     static_cast<unsigned long long>(time_.micros() - started_us));
    }
    else
        logger_.error(kTag, "Asset stream failed");
    refresh_timer_.restart();
    return streamed;
}

void AssetStreamTest::select_next_asset()
{
    selected_ ^= 1U;
}
}  // namespace cyd_asset_stream_smoke
