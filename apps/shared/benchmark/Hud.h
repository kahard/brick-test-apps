#pragma once

#include <cstddef>
#include <cstdint>

#include "FrameStats.h"
#include "lvgl.h"

namespace benchmark_demo
{

/// The bar at the top: which scene runs and its frames per second, time
/// per frame and CPU load over the last half second.
class Hud final
{
public:
    void create(lv_obj_t* screen, std::int32_t width, std::int32_t height, const lv_font_t* font);
    void show_scene(std::size_t number, std::size_t count, const char* name);
    void show_title(const char* title);
    void show_result(const FrameStats::Result& result);

private:
    lv_obj_t* title_   = nullptr;
    lv_obj_t* numbers_ = nullptr;
};

}  // namespace benchmark_demo
