#pragma once

#include "StripeBuffer.h"
#include "TouchInput.h"
#include "Types.h"
#include "brick/core/image/AssetStreamer.h"
#include "brick/core/time/Timer.h"
#include "brick/platform/esp32/PartitionAssetSource.h"

namespace cyd_asset_stream_smoke
{
class AssetStreamTest final
{
public:
    explicit AssetStreamTest(Board& board);
    bool initialize();
    void update();

private:
    bool show_selected_asset();
    void select_next_asset();

    Board&                                       board_;
    StripeBuffer                                 stripe_;
    brick::platform::esp32::PartitionAssetSource assets_{ "assets" };
    brick::core::image::AssetStreamer            streamer_{ board_.display() };
    brick::core::time::Timer                     refresh_timer_{ board_.time() };
    TouchInput                                   touch_{ board_.touch() };
    std::uint8_t                                 selected_ = 0;
};
}  // namespace cyd_asset_stream_smoke
