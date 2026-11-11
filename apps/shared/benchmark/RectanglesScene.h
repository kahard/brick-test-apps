#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "IScene.h"
#include "SceneTools.h"

namespace benchmark_demo
{

/// Coloured rectangles bouncing around: plain fills, rounded corners and
/// half-transparent ones that blend with whatever lies below.
class RectanglesScene final : public IScene
{
public:
    const char* name() const override { return "Rectangles"; }
    void        create(lv_obj_t* parent) override;
    void        step(std::uint32_t elapsed_ms) override;
    void        destroy() override;

private:
    static constexpr std::size_t kRectangles = 16;

    lv_obj_t*                          root_ = nullptr;
    std::array<lv_obj_t*, kRectangles> boxes_{};
    std::array<Bouncer, kRectangles>   motion_{};
};

}  // namespace benchmark_demo
