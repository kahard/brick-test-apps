#pragma once

#include <cstdint>

#include "BenchmarkServices.h"

/// Peripherals of the CYD the benchmark uses (with or without PSRAM: it
/// needs none).
struct BenchmarkFeatures
{
    static constexpr bool display        = true;
    static constexpr bool touch          = true;
    static constexpr bool backlight      = true;
    static constexpr bool sd             = false;
    static constexpr bool requires_psram = false;
    static constexpr bool logging        = true;
    static constexpr int  log_level      = 0;
};

/// How long each scene runs and how long the summary stays.
inline constexpr std::uint32_t kSceneMs   = 8000;
inline constexpr std::uint32_t kSummaryMs = 15000;

/// The ILI9341 has no scan-out buffers: LVGL draws into two internal-RAM
/// buffers (PARTIAL) and the SPI DMA sends one while LVGL draws the other.
inline constexpr benchmark_demo::DrawMode kDrawMode = benchmark_demo::DrawMode::partial;
