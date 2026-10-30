#include "AssetStreamTest.h"
#include "ProgmemAssetReader.h"
#include "assets/joy_tears_240_rgb565.h"
#include "assets/sweat_smile_240_rgb565.h"
namespace esp12f_st7789_asset_stream_smoke
{
namespace
{
    ProgmemAssetReader reader;

    brick::interfaces::display::ImageAsset to_image_asset(const brick_image_asset_t& asset)
    {
        return { asset.data,      asset.width,
                 asset.height,    static_cast<std::size_t>(asset.width) * asset.bytes_per_pixel,
                 asset.data_size, brick::interfaces::display::PixelFormat::rgb565 };
    }
}  // namespace
AssetStreamTest::AssetStreamTest(DisplayDevice& display, Button& button, TimeProvider& time, Logger& logger)
    : display_(display), button_(button), time_(time), logger_(logger), reader_(&reader), streamer_(display, reader)
{
}
bool AssetStreamTest::initialize()
{
    last_pressed_ = button_.is_pressed();
    return show_asset(selected_);
}
void AssetStreamTest::update()
{
    const bool pressed = button_.is_pressed();
    if (pressed && !last_pressed_)
    {
        selected_ = selected_ == 0 ? 1 : 0;
        show_asset(selected_);
    }
    last_pressed_ = pressed;
    time_.delay_ms(20);
}
bool AssetStreamTest::show_asset(std::uint8_t id)
{
    const auto& source_asset = id == 0 ? brick_joy_tears_240 : brick_sweat_smile_240;
    const auto  asset        = to_image_asset(source_asset);
    const auto  started      = time_.micros();
    const bool  ok           = streamer_.stream(asset, { 0, 0, 240, 240 }, scratch_.data(), scratch_.size());
    if (ok)
        logger_.info("asset_stream", "asset=%s elapsed=%luus", id == 0 ? "joy_tears" : "sweat_smile",
                     static_cast<unsigned long>(time_.micros() - started));
    else
        logger_.error("asset_stream", "streaming failed");
    return ok;
}
}  // namespace esp12f_st7789_asset_stream_smoke
