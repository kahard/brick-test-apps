#pragma once

#include <cstdint>

#include "Services.h"
#include "brick/core/memory/MemoryBlock.h"

namespace cyd_color_profile_asset_smoke
{

/// Shows the asset with the color profile applied right of a divider; dragging
/// moves the divider, a double tap centers it.
class ColorProfileAssetTest final
{
public:
    explicit ColorProfileAssetTest(const Services& services);

    Status initialize();
    void   update();

private:
    Status render();
    void   redraw();
    void   update_divider();

    DisplayDevice&                   display_;
    TimeProvider&                    time_;
    Logger&                          logger_;
    Touchscreen&                     touch_;
    AssetSource&                     assets_;
    brick::core::memory::MemoryBlock stripe_;
    std::uint16_t                    divider_x_    = 160U;
    std::int16_t                     last_touch_x_ = 0;
    std::uint32_t                    last_tap_ms_  = 0U;
    bool                             dragging_     = false;
};

}  // namespace cyd_color_profile_asset_smoke
