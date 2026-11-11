#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "IScene.h"

namespace benchmark_demo
{

/// Text: a grid of counters whose digits all change every frame and a large
/// clock. Glyph rendering with anti-aliasing is the work here.
class TextScene final : public IScene
{
public:
    const char* name() const override { return "Text"; }
    void        create(lv_obj_t* parent) override;
    void        step(std::uint32_t elapsed_ms) override;
    void        destroy() override;

private:
    static constexpr std::size_t kMaxRows    = 12;
    static constexpr std::size_t kMaxColumns = 3;

    lv_obj_t*                                     root_  = nullptr;
    lv_obj_t*                                     clock_ = nullptr;
    std::array<lv_obj_t*, kMaxRows * kMaxColumns> labels_{};
    std::size_t                                   count_   = 0;
    std::uint32_t                                 time_ms_ = 0;
    std::uint32_t                                 counter_ = 0;
};

}  // namespace benchmark_demo
