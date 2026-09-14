#pragma once

#include <cstddef>
#include <cstdint>

namespace cyd_lvgl_asset_smoke
{
class LvglBuffers final
{
public:
    ~LvglBuffers();
    bool          initialize();
    void*         first() const;
    void*         second() const;
    std::uint8_t* image() const;

    static constexpr std::size_t image_bytes() { return 100U * 100U * 2U; }
    static constexpr std::size_t frame_bytes() { return 320U * 240U * 2U; }

private:
    std::uint8_t* image_  = nullptr;
    void*         first_  = nullptr;
    void*         second_ = nullptr;
};
}  // namespace cyd_lvgl_asset_smoke
