#pragma once

#include "ApplicationConfig.h"
#include "brick/boards/esp32/wroom32/CydBoard.h"

namespace cyd_lvgl_asset_smoke
{
using Board = brick::boards::CydBoardTemplate<LvglFeatures>;
}  // namespace cyd_lvgl_asset_smoke
