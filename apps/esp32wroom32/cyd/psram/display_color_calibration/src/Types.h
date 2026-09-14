#pragma once

#include "ApplicationConfig.h"
#include "brick/boards/esp32/wroom32/CydBoard.h"

namespace cyd_color_calibration
{

using Board = brick::platform::esp32::CydBoardTemplate<CalibrationFeatures>;

}  // namespace cyd_color_calibration
