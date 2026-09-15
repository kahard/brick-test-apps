#pragma once
#include "Types.h"
#include "brick/core/image/AssetStreamer.h"
#include <array>
#include <cstdint>
namespace esp12f_st7789_asset_stream_smoke
{
class ProgmemAssetReader;
class AssetStreamTest final
{
public:
    AssetStreamTest(DisplayDevice&, Button&, TimeProvider&, Logger&);
    bool initialize();
    void update();

private:
    bool                                      show_asset(std::uint8_t);
    DisplayDevice&                            display_;
    Button&                                   button_;
    TimeProvider&                             time_;
    Logger&                                   logger_;
    ProgmemAssetReader*                       reader_;
    brick::core::image::AssetStreamer         streamer_;
    std::array<std::uint8_t, 240U * 20U * 2U> scratch_{};
    bool                                      last_pressed_ = false;
    std::uint8_t                              selected_     = 0;
};
}  // namespace esp12f_st7789_asset_stream_smoke
