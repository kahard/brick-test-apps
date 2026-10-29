#pragma once

#include "ApplicationConfig.h"
#include "brick/boards/esp32/wroom32/CydBoard.h"

namespace cyd_asset_stream_smoke
{
using Board = brick::boards::CydTemplate<AssetFeatures>;
}  // namespace cyd_asset_stream_smoke
