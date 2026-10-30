#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace cyd_lvgl_asset_smoke
{
class LvglBuffers final
{
private:
    static constexpr std::size_t kImageBytes = 100U * 100U * 2U;
    static constexpr std::size_t kDrawBytes  = 320U * 32U * 2U;

public:
    void*         draw() const;
    std::uint8_t* image();

    static constexpr std::size_t image_bytes() { return kImageBytes; }
    static constexpr std::size_t draw_bytes() { return kDrawBytes; }

private:
    alignas(4) std::array<std::uint8_t, kDrawBytes> draw_{};
    alignas(4) std::array<std::uint8_t, kImageBytes> image_{};
};
}  // namespace cyd_lvgl_asset_smoke
