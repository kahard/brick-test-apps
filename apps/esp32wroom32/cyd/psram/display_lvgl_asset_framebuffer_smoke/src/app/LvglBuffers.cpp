#include "LvglBuffers.h"

namespace cyd_lvgl_asset_smoke
{
LvglBuffers::LvglBuffers(MemoryProvider& memory) : image_(memory), first_(memory), second_(memory)
{
}

Status LvglBuffers::initialize()
{
    using brick::interfaces::memory::MemoryKind;
    if (const Status status = image_.allocate(image_bytes(), MemoryKind::external); !is_ok(status))
        return status;
    if (const Status status = first_.allocate(frame_bytes(), MemoryKind::external); !is_ok(status))
        return status;
    return second_.allocate(frame_bytes(), MemoryKind::external);
}
}  // namespace cyd_lvgl_asset_smoke
