#pragma once

#include <cstdint>

#include "IScene.h"

namespace benchmark_demo
{

/// The worst case: the whole area is a gradient whose colours change every
/// frame, so every pixel is drawn and sent again.
class FullScreenScene final : public IScene
{
public:
    const char* name() const override { return "Full screen"; }
    void        create(lv_obj_t* parent) override;
    void        step(std::uint32_t elapsed_ms) override;
    void        destroy() override;

private:
    lv_obj_t*     root_   = nullptr;
    lv_obj_t*     label_  = nullptr;
    std::uint32_t phase_  = 0;
    std::uint32_t frames_ = 0;
};

}  // namespace benchmark_demo
