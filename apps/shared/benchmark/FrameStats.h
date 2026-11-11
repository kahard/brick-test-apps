#pragma once

#include <cstdint>

#include "brick/interfaces/time/ITimeProvider.h"
#include "lvgl.h"

namespace benchmark_demo
{

/// Measures the frames LVGL renders and how much of the time the
/// application works. Waits for the display (DMA transfer, VSYNC) are not
/// work: LVGL reports them with FLUSH_WAIT events and the drivers sleep
/// meanwhile, so the load is the CPU time drawing needs, on any board.
class FrameStats final
{
public:
    struct Result
    {
        std::uint32_t frames   = 0;
        std::uint32_t fps_x100 = 0;  ///< frames per second x 100
        std::uint32_t frame_us = 0;  ///< average time from one frame to the next
        std::uint32_t load_pct = 0;  ///< share of the time spent working
    };

    explicit FrameStats(brick::interfaces::time::ITimeProvider& time) : time_(time) {}

    /// Counts what LVGL renders on display.
    void attach(lv_display_t* display);
    /// Called around each application update; the time between updates is idle.
    void begin_update();
    void end_update();

    /// The window is the last fraction of a second (HUD), the scene the
    /// whole scene after its warm-up.
    void   restart_window();
    void   restart_scene();
    Result window() const { return result_(window_); }
    Result scene() const { return result_(scene_); }

private:
    struct Accumulator
    {
        std::uint64_t start_us = 0;
        std::uint64_t busy_us  = 0;
        std::uint32_t frames   = 0;
    };

    static void on_render_ready_(lv_event_t* event);
    static void on_wait_start_(lv_event_t* event);
    static void on_wait_finish_(lv_event_t* event);
    Result      result_(const Accumulator& accumulator) const;

    brick::interfaces::time::ITimeProvider& time_;
    Accumulator                             window_;
    Accumulator                             scene_;
    std::uint64_t                           update_start_us_ = 0;
    std::uint64_t                           wait_start_us_   = 0;
    std::uint64_t                           waited_us_       = 0;
};

}  // namespace benchmark_demo
