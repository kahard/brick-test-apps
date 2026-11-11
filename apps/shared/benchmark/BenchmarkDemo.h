#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

#include "BenchmarkServices.h"
#include "FrameStats.h"
#include "FullScreenScene.h"
#include "Hud.h"
#include "ImagesScene.h"
#include "MixedScene.h"
#include "RectanglesScene.h"
#include "SpriteImage.h"
#include "SummaryView.h"
#include "TextScene.h"
#include "WidgetsScene.h"
#include "brick/core/memory/MemoryBlock.h"
#include "brick/lvgl/LvglDisplayAdapter.h"
#include "brick/lvgl/LvglTouchAdapter.h"

namespace benchmark_demo
{

/// The benchmark demo: runs the scenes one after another (a touch skips to
/// the next one), shows frames per second, time per frame and CPU load at
/// the top, logs every scene's result and ends a round with a summary.
/// LVGL draws as fast as the display takes frames.
class BenchmarkDemo final
{
public:
    BenchmarkDemo(const Services& services, const Settings& settings);

    Status initialize();
    void   update();

private:
    static constexpr std::size_t kSceneCount = 6;

    enum class Phase : std::uint8_t
    {
        scene,
        summary,
    };

    Status      create_display_();
    Status      allocate_draw_buffers_(std::size_t& bytes);
    const char* mode_name_() const;
    void        create_screen_();
    void        advance_(std::uint32_t now_ms);
    void        refresh_hud_(std::uint32_t now_ms);
    void        start_scene_(std::size_t index);
    void        finish_scene_();
    void        start_summary_();
    void        finish_summary_();
    static void clicked_(lv_event_t* event);

    Services                                     services_;
    Settings                                     settings_;
    brick::lvgl::LvglDisplayAdapter              display_adapter_;
    std::optional<brick::lvgl::LvglTouchAdapter> touch_adapter_;
    brick::core::memory::MemoryBlock             draw_buffers_[2];
    SpriteImage                                  sprite_;
    WidgetsScene                                 widgets_;
    TextScene                                    text_;
    ImagesScene                                  images_;
    RectanglesScene                              rectangles_;
    FullScreenScene                              full_screen_;
    MixedScene                                   mixed_;
    std::array<IScene*, kSceneCount>             scenes_;
    std::array<SceneResult, kSceneCount>         results_{};
    FrameStats                                   stats_;
    Hud                                          hud_;
    SummaryView                                  summary_;
    const lv_font_t*                             font_           = nullptr;
    lv_display_t*                                display_        = nullptr;
    lv_obj_t*                                    stage_          = nullptr;
    Phase                                        phase_          = Phase::scene;
    std::size_t                                  scene_index_    = 0;
    DrawMode                                     mode_           = DrawMode::frame_buffers;
    std::uint32_t                                last_ms_        = 0;
    std::uint32_t                                phase_start_ms_ = 0;
    std::uint32_t                                hud_ms_         = 0;
    std::uint32_t                                round_          = 1;
    bool                                         measuring_      = false;
    bool                                         next_requested_ = false;
};

}  // namespace benchmark_demo
