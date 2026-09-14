#pragma once

#include "ApplicationConfig.h"
#include "brick/boards/esp32/wroom32/CydBoard.h"

namespace cyd_sd_card_filesystem_touch_smoke
{
using Board = brick::platform::esp32::CydBoardTemplate<SdFeatures>;
}  // namespace cyd_sd_card_filesystem_touch_smoke
