#include "BenchmarkDemo.h"

#include "SceneTools.h"

namespace benchmark_demo
{

namespace
{
    constexpr char kTag[] = "benchmark";
    /// LVGL may draw a frame at every update; the display paces it.
    constexpr std::uint32_t kRefreshPeriodMs = 1;
    /// The first half second of a scene (creation, first full redraw) is not measured.
    constexpr std::uint32_t kWarmupMs       = 500;
    constexpr std::uint32_t kHudPeriodMs    = 500;
    constexpr std::int32_t  kHudPadding     = 6;
    constexpr std::int32_t  kSpriteFraction = 8;  ///< the ball is 1/8 of the shorter screen side
    constexpr std::size_t   kBytesPerPixel  = 2;
    constexpr std::size_t   kMinAlignment   = 64;
    constexpr std::uint32_t kHundred        = 100;
    constexpr std::uint32_t kThousand       = 1000;
}  // namespace

BenchmarkDemo::BenchmarkDemo(const Services& services, const Settings& settings)
    : services_(services), settings_(settings), display_adapter_(services.display),
      draw_buffers_{ brick::core::memory::MemoryBlock{ services.memory },
                     brick::core::memory::MemoryBlock{ services.memory } },
      sprite_(services.memory), images_(sprite_), mixed_({ &widgets_, &text_, &images_, &rectangles_ }),
      scenes_{ &widgets_, &text_, &images_, &rectangles_, &full_screen_, &mixed_ }, stats_(services.time)
{
}

Status BenchmarkDemo::initialize()
{
    lv_init();
    if (const Status status = create_display_(); !is_ok(status))
        return status;
    const brick::interfaces::display::DisplaySize size = services_.display.size();
    const std::int32_t shorter = static_cast<std::int32_t>(size.width < size.height ? size.width : size.height);
    if (const Status status = sprite_.initialize(shorter / kSpriteFraction); !is_ok(status))
        return status;
    if (services_.touch != nullptr)
    {
        touch_adapter_.emplace(*services_.touch);
        if (touch_adapter_->create() == nullptr)
            return Status::error;
    }
    stats_.attach(display_);
    lv_timer_set_period(lv_display_get_refr_timer(display_), kRefreshPeriodMs);
    create_screen_();
    last_ms_ = services_.time.millis();
    services_.logger.info(kTag, "Benchmark demo board=%s screen=%ux%u mode=%s", settings_.board,
                          static_cast<unsigned>(size.width), static_cast<unsigned>(size.height), mode_name_());
    start_scene_(0);
    return Status::ok;
}

Status BenchmarkDemo::create_display_()
{
    mode_ = services_.frame_buffers != nullptr ? settings_.draw_mode : DrawMode::partial;
    if (mode_ == DrawMode::frame_buffers)
    {
        display_ = display_adapter_.create_framebuffer(*services_.frame_buffers);
        return display_ != nullptr ? Status::ok : Status::error;
    }
    std::size_t bytes = 0;
    if (const Status status = allocate_draw_buffers_(bytes); !is_ok(status))
        return status;
    const auto size = static_cast<std::uint32_t>(bytes);
    if (mode_ == DrawMode::ram_and_flip)
        display_ = display_adapter_.create_buffered(*services_.frame_buffers, draw_buffers_[0].data(),
                                                    draw_buffers_[1].data(), size);
    else
        display_ = display_adapter_.create(LV_DISPLAY_RENDER_MODE_PARTIAL, draw_buffers_[0].data(),
                                           draw_buffers_[1].data(), size);
    return display_ != nullptr ? Status::ok : Status::error;
}

Status BenchmarkDemo::allocate_draw_buffers_(std::size_t& bytes)
{
    const brick::interfaces::display::DisplaySize size = services_.display.size();
    bytes = static_cast<std::size_t>(size.width) * settings_.partial_rows * kBytesPerPixel;
    // Whole cache lines: the DMA, the PPA and LVGL's own alignment need them.
    const std::size_t reported  = services_.display.capabilities().dma_alignment_bytes;
    const std::size_t alignment = reported > kMinAlignment ? reported : kMinAlignment;
    for (brick::core::memory::MemoryBlock& buffer : draw_buffers_)
        if (const Status status = buffer.allocate(bytes, settings_.partial_memory, alignment); !is_ok(status))
            return status;
    return Status::ok;
}

const char* BenchmarkDemo::mode_name_() const
{
    switch (mode_)
    {
        case DrawMode::frame_buffers:
            return "DIRECT frame buffers";
        case DrawMode::ram_and_flip:
            return "RAM buffers + page flip";
        default:
            return "PARTIAL buffers";
    }
}

void BenchmarkDemo::create_screen_()
{
    const brick::interfaces::display::DisplaySize size   = services_.display.size();
    const auto                                    width  = static_cast<std::int32_t>(size.width);
    const auto                                    height = static_cast<std::int32_t>(size.height);
    font_                                                = large_font(width);
    lv_obj_t* screen                                     = lv_screen_active();
    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);
    lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
    const std::int32_t hud_height = lv_font_get_line_height(font_) + 2 * kHudPadding;
    hud_.create(screen, width, hud_height, font_);
    stage_ = create_box(screen, 0, hud_height, width, height - hud_height);
    // A touch anywhere skips to the next scene: a transparent catcher on the top layer.
    lv_obj_t* catcher = create_box(lv_layer_top(), 0, 0, width, height);
    lv_obj_add_flag(catcher, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(catcher, clicked_, LV_EVENT_CLICKED, this);
}

void BenchmarkDemo::update()
{
    stats_.begin_update();
    const std::uint32_t now     = services_.time.millis();
    const std::uint32_t elapsed = now - last_ms_;
    last_ms_                    = now;
    lv_tick_inc(elapsed);
    if (phase_ == Phase::scene)
        scenes_[scene_index_]->step(elapsed);
    advance_(now);
    refresh_hud_(now);
    lv_timer_handler();
    stats_.end_update();
}

void BenchmarkDemo::advance_(std::uint32_t now_ms)
{
    const std::uint32_t in_phase  = now_ms - phase_start_ms_;
    const bool          requested = next_requested_;
    next_requested_               = false;
    if (phase_ == Phase::summary)
    {
        if (requested || in_phase >= settings_.summary_ms)
            finish_summary_();
        return;
    }
    if (!measuring_ && in_phase >= kWarmupMs)
    {
        stats_.restart_scene();
        measuring_ = true;
    }
    if (requested || in_phase >= settings_.scene_ms)
        finish_scene_();
}

void BenchmarkDemo::refresh_hud_(std::uint32_t now_ms)
{
    if (now_ms - hud_ms_ < kHudPeriodMs)
        return;
    hud_ms_ = now_ms;
    hud_.show_result(stats_.window());
    stats_.restart_window();
}

void BenchmarkDemo::start_scene_(std::size_t index)
{
    scene_index_    = index;
    phase_          = Phase::scene;
    phase_start_ms_ = last_ms_;
    measuring_      = false;
    scenes_[index]->create(stage_);
    hud_.show_scene(index + 1, kSceneCount, scenes_[index]->name());
}

void BenchmarkDemo::finish_scene_()
{
    IScene*      scene  = scenes_[scene_index_];
    SceneResult& result = results_[scene_index_];
    result              = { scene->name(), measuring_ ? stats_.scene() : FrameStats::Result{} };
    services_.logger.info(kTag, "round=%u scene=%s frames=%u fps=%u.%02u frame_ms=%u.%02u cpu=%u%%",
                          static_cast<unsigned>(round_), result.name, static_cast<unsigned>(result.result.frames),
                          static_cast<unsigned>(result.result.fps_x100 / kHundred),
                          static_cast<unsigned>(result.result.fps_x100 % kHundred),
                          static_cast<unsigned>(result.result.frame_us / kThousand),
                          static_cast<unsigned>(result.result.frame_us % kThousand / (kThousand / kHundred)),
                          static_cast<unsigned>(result.result.load_pct));
    scene->destroy();
    if (scene_index_ + 1 < kSceneCount)
        start_scene_(scene_index_ + 1);
    else
        start_summary_();
}

void BenchmarkDemo::start_summary_()
{
    phase_          = Phase::summary;
    phase_start_ms_ = last_ms_;
    summary_.create(stage_, settings_.board, results_.data(), kSceneCount, font_);
    hud_.show_title("Summary");
    const FrameStats::Result mean = mean_result(results_.data(), kSceneCount);
    lv_mem_monitor_t         memory{};
    lv_mem_monitor(&memory);
    services_.logger.info(kTag, "round=%u board=%s mean_fps=%u.%02u mean_cpu=%u%% lvgl_memory_used=%u%%",
                          static_cast<unsigned>(round_), settings_.board,
                          static_cast<unsigned>(mean.fps_x100 / kHundred),
                          static_cast<unsigned>(mean.fps_x100 % kHundred), static_cast<unsigned>(mean.load_pct),
                          static_cast<unsigned>(memory.used_pct));
}

void BenchmarkDemo::finish_summary_()
{
    summary_.destroy();
    ++round_;
    start_scene_(0);
}

void BenchmarkDemo::clicked_(lv_event_t* event)
{
    static_cast<BenchmarkDemo*>(lv_event_get_user_data(event))->next_requested_ = true;
}

}  // namespace benchmark_demo
