#pragma once

#include <cstdint>

#include "Services.h"
#include "TouchInput.h"
#include "brick/core/image/AssetStreamer.h"
#include "brick/core/memory/MemoryBlock.h"
#include "brick/core/time/Timer.h"

namespace cyd_asset_stream_smoke
{
/// Streams the bundled images straight to the panel through one internal DMA
/// stripe; each touch switches the image.
class AssetStreamTest final
{
public:
    explicit AssetStreamTest(const Services& services);
    Status initialize();
    void   update();

private:
    Status show_selected_asset();
    void   report(Status status);
    void   select_next_asset();

    TimeProvider&                     time_;
    Logger&                           logger_;
    AssetSource&                      assets_;
    brick::core::memory::MemoryBlock  stripe_;
    brick::core::image::AssetStreamer streamer_;
    brick::core::time::Timer          refresh_timer_;
    TouchInput                        touch_;
    std::uint8_t                      selected_ = 0;
};
}  // namespace cyd_asset_stream_smoke
