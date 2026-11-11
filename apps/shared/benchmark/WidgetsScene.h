#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "IScene.h"

namespace benchmark_demo
{

/// Standard widgets that change every frame: two arcs, a spinner, three bars,
/// a slider and a line chart that scrolls. Many small areas are redrawn.
class WidgetsScene final : public IScene
{
public:
    const char* name() const override { return "Widgets"; }
    void        create(lv_obj_t* parent) override;
    void        step(std::uint32_t elapsed_ms) override;
    void        destroy() override;

private:
    static constexpr std::size_t kArcs = 2;
    static constexpr std::size_t kBars = 3;

    void create_top_row_(std::int32_t width, std::int32_t height);
    void create_bottom_row_(std::int32_t top, std::int32_t width, std::int32_t height);

    lv_obj_t*                    root_ = nullptr;
    std::array<lv_obj_t*, kArcs> arcs_{};
    std::array<lv_obj_t*, kBars> bars_{};
    lv_obj_t*                    slider_  = nullptr;
    lv_obj_t*                    chart_   = nullptr;
    lv_chart_series_t*           series_  = nullptr;
    std::uint32_t                time_ms_ = 0;
};

}  // namespace benchmark_demo
