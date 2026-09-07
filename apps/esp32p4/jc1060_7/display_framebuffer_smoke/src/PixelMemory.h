#pragma once
#include <cstddef>
#include <cstdint>

// Explicit PSRAM allocation is isolated here. Never consume internal RAM as a
// fallback for full-screen buffers or an asset bundle.
class PixelMemory final
{
public:
    PixelMemory() = default;
    ~PixelMemory();
    PixelMemory(const PixelMemory&)             = delete;
    PixelMemory&  operator=(const PixelMemory&) = delete;
    bool          allocate(std::size_t bytes);
    std::uint8_t* data() const { return data_; }
    std::size_t   size() const { return size_; }

private:
    std::uint8_t* data_ = nullptr;
    std::size_t   size_ = 0;
};
