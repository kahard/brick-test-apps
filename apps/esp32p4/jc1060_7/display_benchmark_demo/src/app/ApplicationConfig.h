#pragma once

#include <cstdint>

#include "BenchmarkServices.h"

/// Peripherals of the 7-inch panel the benchmark uses.
struct BenchmarkFeatures
{
    static constexpr bool display   = true;
    static constexpr bool touch     = true;
    static constexpr bool backlight = true;
    static constexpr bool sd        = false;
    static constexpr bool logging   = true;
    static constexpr int  log_level = 0;
};

/// How long each scene runs and how long the summary stays.
inline constexpr std::uint32_t kSceneMs   = 8000;
inline constexpr std::uint32_t kSummaryMs = 15000;

/// Where LVGL draws (benchmark_demo::DrawMode): into internal RAM with a page
/// flip (the default: drawing in PSRAM takes longer than a refresh), straight
/// into the panel's scan-out buffers, or into internal RAM copied into the
/// buffer on the screen (tears). The README compares them.
inline constexpr benchmark_demo::DrawMode kDrawMode = benchmark_demo::DrawMode::ram_and_flip;
