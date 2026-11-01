#pragma once

#include "FrameBuffers.h"
#include "Services.h"
#include "brick/lvgl/LvglDisplayAdapter.h"

namespace lvgl_benchmark_smoke
{
/// The only display-specific LVGL setup: FULL render buffers + MIPI transfer.
/// Unlike the S3 RGB driver, MipiDsiDisplay does not expose IFrameBufferDisplay.
class LvglRuntime final
{
public:
    LvglRuntime(DisplayDevice& display, MemoryProvider& memory, TimeProvider& time);
    Status        initialize();
    void          update();
    lv_display_t* display() const { return display_; }

private:
    FrameBuffers                    buffers_;
    brick::lvgl::LvglDisplayAdapter adapter_;
    TimeProvider&                   time_;
    lv_display_t*                   display_       = nullptr;
    std::uint32_t                   previous_tick_ = 0;
};
}  // namespace lvgl_benchmark_smoke
