#pragma once

#include "StripeBuffer.h"
#include "TouchInput.h"
#include "brick/core/image/AssetStreamer.h"
#include "brick/core/time/Timer.h"
#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/display/ITouchscreen.h"
#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/time/ITimeProvider.h"
#include "brick/platform/esp32/PartitionAssetSource.h"

namespace cyd_asset_stream_smoke
{
class AssetStreamTest final
{
public:
    AssetStreamTest(brick::interfaces::display::IDisplayDevice& display, brick::interfaces::display::ITouchscreen& touch,
                    brick::interfaces::time::ITimeProvider& time, brick::interfaces::logging::ILogger& logger);
    bool initialize();
    void update();

private:
    bool show_selected_asset();
    void select_next_asset();

    brick::interfaces::time::ITimeProvider&     time_;
    brick::interfaces::logging::ILogger&        logger_;
    StripeBuffer                                 stripe_;
    brick::platform::esp32::PartitionAssetSource assets_{ "assets" };
    brick::core::image::AssetStreamer            streamer_;
    brick::core::time::Timer                     refresh_timer_;
    TouchInput                                   touch_;
    std::uint8_t                                 selected_ = 0;
};
}  // namespace cyd_asset_stream_smoke
