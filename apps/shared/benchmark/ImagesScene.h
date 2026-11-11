#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "IScene.h"
#include "SceneTools.h"
#include "SpriteImage.h"

namespace benchmark_demo
{

/// Images with a transparent edge bouncing over a dark background: image
/// copies with alpha blending, the old and the new place of each ball
/// redrawn every frame.
class ImagesScene final : public IScene
{
public:
    explicit ImagesScene(const SpriteImage& sprite) : sprite_(sprite) {}

    const char* name() const override { return "Images"; }
    void        create(lv_obj_t* parent) override;
    void        step(std::uint32_t elapsed_ms) override;
    void        destroy() override;

private:
    static constexpr std::size_t kSprites = 10;

    const SpriteImage&              sprite_;
    lv_obj_t*                       root_ = nullptr;
    std::array<lv_obj_t*, kSprites> images_{};
    std::array<Bouncer, kSprites>   motion_{};
};

}  // namespace benchmark_demo
