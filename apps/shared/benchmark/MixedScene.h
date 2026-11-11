#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "IScene.h"

namespace benchmark_demo
{

/// Four scenes at once, one in each quarter of the area: the load of a busy
/// real screen with widgets, text, images and moving shapes together.
class MixedScene final : public IScene
{
public:
    static constexpr std::size_t kTiles = 4;

    /// The scenes in reading order: top left, top right, bottom left, bottom right.
    explicit MixedScene(const std::array<IScene*, kTiles>& scenes) : scenes_(scenes) {}

    const char* name() const override { return "Mixed"; }
    void        create(lv_obj_t* parent) override;
    void        step(std::uint32_t elapsed_ms) override;
    void        destroy() override;

private:
    std::array<IScene*, kTiles> scenes_;
    lv_obj_t*                   root_ = nullptr;
};

}  // namespace benchmark_demo
