#pragma once

#include "ApplicationConfig.h"
#include "brick/boards/esp32/wroom32/CydBoard.h"

namespace cyd_color_profile_asset_smoke
{

using Board = brick::platform::esp32::CydBoardTemplate<ColorProfileAssetFeatures>;

}  // namespace cyd_color_profile_asset_smoke
