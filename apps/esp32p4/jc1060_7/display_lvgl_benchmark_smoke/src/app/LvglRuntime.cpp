#include "LvglRuntime.h"

namespace lvgl_benchmark_smoke
{
LvglRuntime::LvglRuntime(DisplayDevice& display, MemoryProvider& memory, TimeProvider& time)
    : buffers_(display, memory), adapter_(display), time_(time)
{
}

Status LvglRuntime::initialize()
{
    if (const Status status = buffers_.initialize(); !is_ok(status))
        return status;
    lv_init();
    display_       = adapter_.create(LV_DISPLAY_RENDER_MODE_FULL, buffers_.buffer(0).data, buffers_.buffer(1).data,
                                     buffers_.size_bytes());
    previous_tick_ = time_.millis();
    return display_ != nullptr ? Status::ok : Status::error;
}

void LvglRuntime::update()
{
    const std::uint32_t now = time_.millis();
    // Use actual elapsed time, including render latency, not an assumed 10 ms.
    lv_tick_inc(now - previous_tick_);
    previous_tick_ = now;
    lv_timer_handler();
}
}  // namespace lvgl_benchmark_smoke
