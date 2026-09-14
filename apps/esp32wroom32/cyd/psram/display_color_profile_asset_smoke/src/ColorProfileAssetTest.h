#pragma once

#include <cstdint>

#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/display/ITouchscreen.h"
#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/time/ITimeProvider.h"

namespace cyd_color_profile_asset_smoke
{

class ColorProfileAssetTest final
{
public:
    ColorProfileAssetTest(brick::interfaces::display::IDisplayDevice& display,
                          brick::interfaces::time::ITimeProvider& time,
                          brick::interfaces::logging::ILogger& logger,
                          brick::interfaces::display::ITouchscreen& touch);

    bool initialize();
    void update();

private:
    bool render();
    void update_divider();

    brick::interfaces::display::IDisplayDevice& display_;
    brick::interfaces::time::ITimeProvider& time_;
    brick::interfaces::logging::ILogger& logger_;
    brick::interfaces::display::ITouchscreen& touch_;
    std::uint16_t divider_x_ = 160U;
    std::int16_t last_touch_x_ = 0;
    std::uint32_t last_tap_ms_ = 0U;
    bool dragging_ = false;
};

}  // namespace cyd_color_profile_asset_smoke
