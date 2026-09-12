#include "PixelMemory.h"

#include "esp_heap_caps.h"

namespace display_touch_smoke
{
PixelMemory::~PixelMemory()
{
    heap_caps_free(data_);
}

bool PixelMemory::allocate(std::size_t bytes)
{
    if (data_ != nullptr || bytes == 0)
        return false;

    data_ = static_cast<std::uint8_t*>(heap_caps_aligned_alloc(64, bytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
    return data_ != nullptr;
}
}  // namespace display_touch_smoke
