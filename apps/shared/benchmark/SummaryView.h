#pragma once

#include <cstddef>
#include <cstdint>

#include "FrameStats.h"
#include "lvgl.h"

namespace benchmark_demo
{

struct SceneResult
{
    const char*        name = "";
    FrameStats::Result result;
};

/// The mean of the measured scenes' results (each scene counts once; one
/// skipped before its warm-up ended does not count).
FrameStats::Result mean_result(const SceneResult* results, std::size_t count);

/// The table shown after the last scene: frames per second, time per frame
/// and CPU load of every scene, and their mean.
class SummaryView final
{
public:
    void create(lv_obj_t* parent, const char* board, const SceneResult* results, std::size_t count,
                const lv_font_t* font);
    void destroy();

private:
    /// Vertical cell padding that fits rows of font into height.
    static std::int32_t fitting_padding_(std::int32_t height, std::int32_t rows, const lv_font_t* font);
    void                fill_row_(std::uint32_t row, const char* name, const FrameStats::Result& result);

    lv_obj_t* root_  = nullptr;
    lv_obj_t* table_ = nullptr;
};

}  // namespace benchmark_demo
