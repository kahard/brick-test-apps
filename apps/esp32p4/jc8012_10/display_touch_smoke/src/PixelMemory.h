#pragma once

#include <cstddef>
#include <cstdint>

namespace display_touch_smoke
{
// Full logical frames belong in PSRAM. Internal RAM is intentionally never a
// fallback: a failed allocation is reported to the application instead.
class PixelMemory final
{
public:
    PixelMemory() = default;
    ~PixelMemory();
    PixelMemory(const PixelMemory&)            = delete;
    PixelMemory& operator=(const PixelMemory&) = delete;

    bool          allocate(std::size_t bytes);
    std::uint8_t* data() const { return data_; }

private:
    std::uint8_t* data_ = nullptr;
};
}  // namespace display_touch_smoke
