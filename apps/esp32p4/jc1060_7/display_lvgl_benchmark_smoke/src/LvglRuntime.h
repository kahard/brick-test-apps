#pragma once
#include "FrameBuffers.h"
#include "brick/interfaces/time/ITimeProvider.h"
#include "brick/platform/esp32/LvglDisplayAdapter.h"

// The only display-specific LVGL setup: FULL render buffers + MIPI transfer.
// Unlike the S3 RGB driver, MipiDsiDisplay does not expose IFrameBufferDisplay.
class LvglRuntime final
{
public:
    LvglRuntime(brick::interfaces::display::IDisplayDevice& display, brick::interfaces::time::ITimeProvider& time);
    bool          initialize();
    void          update();
    lv_display_t* display() const { return display_; }

private:
    FrameBuffers                               buffers_;
    brick::platform::esp32::LvglDisplayAdapter adapter_;
    brick::interfaces::time::ITimeProvider&    time_;
    lv_display_t*                              display_       = nullptr;
    std::uint32_t                              previous_tick_ = 0;
};
