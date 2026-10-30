#pragma once

#include <cstddef>
#include <cstdint>

#include "Services.h"
#include "brick/core/memory/MemoryBlock.h"

namespace cyd_lvgl_asset_smoke
{
/// The current image and two full-screen LVGL render buffers in PSRAM.
class LvglBuffers final
{
public:
    explicit LvglBuffers(MemoryProvider& memory);
    bool          initialize();
    void*         first() const { return first_.data(); }
    void*         second() const { return second_.data(); }
    std::uint8_t* image() const { return image_.data(); }

    static constexpr std::size_t image_bytes() { return 100U * 100U * 2U; }
    static constexpr std::size_t frame_bytes() { return 320U * 240U * 2U; }

private:
    brick::core::memory::MemoryBlock image_;
    brick::core::memory::MemoryBlock first_;
    brick::core::memory::MemoryBlock second_;
};
}  // namespace cyd_lvgl_asset_smoke
