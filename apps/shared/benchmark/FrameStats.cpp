#include "FrameStats.h"

namespace benchmark_demo
{

namespace
{
    constexpr std::uint64_t kHundredMillion = 100000000ULL;  ///< microseconds per second x 100
    constexpr std::uint64_t kPercent        = 100;

    FrameStats* stats_of(lv_event_t* event)
    {
        return static_cast<FrameStats*>(lv_event_get_user_data(event));
    }
}  // namespace

void FrameStats::attach(lv_display_t* display)
{
    lv_display_add_event_cb(display, on_render_ready_, LV_EVENT_RENDER_READY, this);
    lv_display_add_event_cb(display, on_wait_start_, LV_EVENT_FLUSH_WAIT_START, this);
    lv_display_add_event_cb(display, on_wait_finish_, LV_EVENT_FLUSH_WAIT_FINISH, this);
    restart_window();
    restart_scene();
}

void FrameStats::begin_update()
{
    update_start_us_ = time_.micros();
    waited_us_       = 0;
}

void FrameStats::end_update()
{
    const std::uint64_t spent = time_.micros() - update_start_us_;
    const std::uint64_t busy  = spent > waited_us_ ? spent - waited_us_ : 0;
    window_.busy_us += busy;
    scene_.busy_us += busy;
}

void FrameStats::restart_window()
{
    window_ = { time_.micros(), 0, 0 };
}

void FrameStats::restart_scene()
{
    scene_ = { time_.micros(), 0, 0 };
}

FrameStats::Result FrameStats::result_(const Accumulator& accumulator) const
{
    const std::uint64_t wall = time_.micros() - accumulator.start_us;
    Result              result;
    result.frames = accumulator.frames;
    if (wall == 0)
        return result;
    result.fps_x100          = static_cast<std::uint32_t>(accumulator.frames * kHundredMillion / wall);
    result.frame_us          = accumulator.frames != 0 ? static_cast<std::uint32_t>(wall / accumulator.frames) : 0;
    const std::uint64_t load = accumulator.busy_us * kPercent / wall;
    result.load_pct          = static_cast<std::uint32_t>(load < kPercent ? load : kPercent);
    return result;
}

void FrameStats::on_render_ready_(lv_event_t* event)
{
    FrameStats* self = stats_of(event);
    ++self->window_.frames;
    ++self->scene_.frames;
}

void FrameStats::on_wait_start_(lv_event_t* event)
{
    FrameStats* self     = stats_of(event);
    self->wait_start_us_ = self->time_.micros();
}

void FrameStats::on_wait_finish_(lv_event_t* event)
{
    FrameStats* self = stats_of(event);
    self->waited_us_ += self->time_.micros() - self->wait_start_us_;
}

}  // namespace benchmark_demo
