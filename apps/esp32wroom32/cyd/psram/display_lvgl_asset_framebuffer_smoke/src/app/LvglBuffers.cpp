#include "LvglBuffers.h"

namespace cyd_lvgl_asset_smoke
{
LvglBuffers::LvglBuffers(MemoryProvider& memory) : image_(memory), first_(memory), second_(memory)
{
}

bool LvglBuffers::initialize()
{
    using brick::interfaces::memory::MemoryKind;
    return image_.allocate(image_bytes(), MemoryKind::external) && first_.allocate(frame_bytes(), MemoryKind::external)
           && second_.allocate(frame_bytes(), MemoryKind::external);
}
}  // namespace cyd_lvgl_asset_smoke
