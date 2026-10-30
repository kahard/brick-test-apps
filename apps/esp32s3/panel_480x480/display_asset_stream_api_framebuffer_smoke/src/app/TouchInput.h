#pragma once

#include "Services.h"

namespace asset_stream_smoke
{
/// Reports each new touch once.
class TouchInput final
{
public:
    explicit TouchInput(Touchscreen& touch);

    bool pressed();

private:
    Touchscreen& touch_;
    bool         was_down_ = false;
};
}  // namespace asset_stream_smoke
