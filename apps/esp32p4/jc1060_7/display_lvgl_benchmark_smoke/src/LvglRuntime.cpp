#include "LvglRuntime.h"

LvglRuntime::LvglRuntime(brick::interfaces::display::IDisplayDevice& display,
                         brick::interfaces::time::ITimeProvider&     time)
    : buffers_(display), adapter_(display), time_(time)
{
}

bool LvglRuntime::initialize()
{
    if (!buffers_.initialize())
        return false;
    lv_init();
    display_       = adapter_.create(LV_DISPLAY_RENDER_MODE_FULL, buffers_.buffer(0).data, buffers_.buffer(1).data,
                                     buffers_.size_bytes());
    previous_tick_ = time_.millis();
    return display_ != nullptr;
}

void LvglRuntime::update()
{
    const std::uint32_t now = time_.millis();
    // Use actual elapsed time, including render latency, not an assumed 10 ms.
    lv_tick_inc(now - previous_tick_);
    previous_tick_ = now;
    lv_timer_handler();
}
