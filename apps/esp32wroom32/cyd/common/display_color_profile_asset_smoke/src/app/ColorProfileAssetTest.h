#pragma once

#include <cstdint>

#include "Services.h"
#include "brick/core/input/DragSlider.h"
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
    brick::core::input::DragSlider   divider_;
};

}  // namespace cyd_color_profile_asset_smoke
