#pragma once

#include <cstddef>
#include <cstdint>

#include "BenchmarkServices.h"
#include "brick/core/memory/MemoryBlock.h"
#include "lvgl.h"

namespace benchmark_demo
{

/// A shaded ball with a soft, semi-transparent edge (ARGB8888), drawn once
/// in initialize(): the image scene needs no asset files, so it runs the
/// same on every board.
class SpriteImage final
{
public:
    explicit SpriteImage(brick::interfaces::memory::IMemoryProvider& memory) : pixels_(memory) {}

    /// Allocates and draws a ball diameter pixels wide.
    Status initialize(std::int32_t diameter);

    const lv_image_dsc_t* descriptor() const { return &descriptor_; }
    std::int32_t          diameter() const { return diameter_; }

private:
    static constexpr std::size_t kBytesPerPixel = 4;

    void draw_();
    void draw_pixel_(std::int32_t x, std::int32_t y, std::uint8_t* pixel) const;

    brick::core::memory::MemoryBlock pixels_;
    lv_image_dsc_t                   descriptor_{};
    std::int32_t                     diameter_ = 0;
};

}  // namespace benchmark_demo
