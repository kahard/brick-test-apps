#pragma once

#include "brick/interfaces/display/ITouchscreen.h"

namespace cyd_asset_stream_smoke
{
class TouchInput final
{
public:
    explicit TouchInput(brick::interfaces::display::ITouchscreen& touch);
    bool pressed();

private:
    brick::interfaces::display::ITouchscreen& touch_;
    bool                                      was_down_ = false;
};
}  // namespace cyd_asset_stream_smoke
