#include "LvglBuffers.h"

#include "esp_heap_caps.h"

namespace cyd_lvgl_asset_smoke
{
LvglBuffers::~LvglBuffers()
{
    heap_caps_free(image_);
    heap_caps_free(first_);
    heap_caps_free(second_);
}

bool LvglBuffers::initialize()
{
    image_  = static_cast<std::uint8_t*>(heap_caps_malloc(image_bytes(), MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
    first_  = heap_caps_malloc(frame_bytes(), MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    second_ = heap_caps_malloc(frame_bytes(), MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    return image_ != nullptr && first_ != nullptr && second_ != nullptr;
}

void* LvglBuffers::first() const
{
    return first_;
}
void* LvglBuffers::second() const
{
    return second_;
}
std::uint8_t* LvglBuffers::image() const
{
    return image_;
}
}  // namespace cyd_lvgl_asset_smoke
