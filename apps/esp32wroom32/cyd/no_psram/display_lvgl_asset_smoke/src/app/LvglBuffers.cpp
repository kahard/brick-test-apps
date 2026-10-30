#include "LvglBuffers.h"

namespace cyd_lvgl_asset_smoke
{
void* LvglBuffers::draw() const
{
    return const_cast<std::uint8_t*>(draw_.data());
}

std::uint8_t* LvglBuffers::image()
{
    return image_.data();
}
}  // namespace cyd_lvgl_asset_smoke
