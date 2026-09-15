#pragma once
#include "Types.h"
#include "brick/core/image/AssetStreamer.h"
#include <array>
namespace esp12f_st7789_asset_stream_api_smoke
{
class SolidColorReader final : public brick::interfaces::display::IAssetReader
{
public:
    bool read(const brick::interfaces::display::ImageAsset&, std::size_t, std::uint8_t*, std::size_t) override;
};
class AssetStreamerApiTest final
{
public:
    AssetStreamerApiTest(DisplayDevice&, TimeProvider&, Logger&);
    bool initialize();
    void update();

private:
    bool                                      stream_color(const std::uint8_t* color, const char* name);
    DisplayDevice&                            display_;
    TimeProvider&                             time_;
    Logger&                                   logger_;
    SolidColorReader                          reader_;
    brick::core::image::AssetStreamer         streamer_;
    std::array<std::uint8_t, 240U * 40U * 2U> scratch_{};
    bool                                      blue_          = false;
    std::uint32_t                             next_frame_ms_ = 0;
};
}  // namespace esp12f_st7789_asset_stream_api_smoke
