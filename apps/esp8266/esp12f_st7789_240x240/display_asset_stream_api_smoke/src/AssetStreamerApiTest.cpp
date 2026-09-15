#include "AssetStreamerApiTest.h"
#include <pgmspace.h>
namespace esp12f_st7789_asset_stream_api_smoke
{
namespace
{
    constexpr std::uint8_t red[] PROGMEM  = { 0x00, 0xF8 };
    constexpr std::uint8_t blue[] PROGMEM = { 0x1F, 0x00 };
}  // namespace
bool SolidColorReader::read(const brick::interfaces::display::ImageAsset& asset, std::size_t offset,
                            std::uint8_t* destination, std::size_t bytes)
{
    if (destination == nullptr || asset.data == nullptr || offset > asset.data_size || bytes > asset.data_size - offset)
        return false;
    std::uint8_t color[2] = {};
    memcpy_P(color, asset.data, sizeof(color));
    for (std::size_t index = 0; index < bytes; index += 2)
    {
        destination[index] = color[0];
        if (index + 1 < bytes)
            destination[index + 1] = color[1];
    }
    return true;
}
AssetStreamerApiTest::AssetStreamerApiTest(DisplayDevice& display, TimeProvider& time, Logger& logger)
    : display_(display), time_(time), logger_(logger), streamer_(display, reader_)
{
}
bool AssetStreamerApiTest::initialize()
{
    return stream_color(red, "red");
}
void AssetStreamerApiTest::update()
{
    if (time_.millis() < next_frame_ms_)
        return;
    blue_ = !blue_;
    stream_color(blue_ ? blue : red, blue_ ? "blue" : "red");
    next_frame_ms_ = time_.millis() + 500;
}
bool AssetStreamerApiTest::stream_color(const std::uint8_t* color, const char* name)
{
    const brick::interfaces::display::ImageAsset asset{
        color, 240, 240, 480, 240U * 240U * 2U, brick::interfaces::display::PixelFormat::rgb565
    };
    const auto started = time_.micros();
    const bool ok      = streamer_.stream(asset, { 0, 0, 240, 240 }, scratch_.data(), scratch_.size());
    if (ok)
        logger_.info("asset_api", "asset=%s elapsed=%luus", name, static_cast<unsigned long>(time_.micros() - started));
    else
        logger_.error("asset_api", "AssetStreamer failed");
    return ok;
}
}  // namespace esp12f_st7789_asset_stream_api_smoke
