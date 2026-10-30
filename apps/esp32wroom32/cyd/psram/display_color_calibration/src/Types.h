#pragma once

#include "ApplicationConfig.h"
#include "brick/boards/esp32/wroom32/CydBoard.h"

namespace cyd_color_calibration
{

using Board = brick::boards::CydBoardTemplate<CalibrationFeatures>;

}  // namespace cyd_color_calibration
