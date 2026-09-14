#pragma once

#include "CalibrationTest.h"
#include "Types.h"

namespace cyd_color_calibration
{

class Application final
{
public:
    bool initialize();
    void update();

private:
    Board           board_;
    CalibrationTest test_{ board_.display(), board_.backlight(), board_.time(), board_.logger(), board_.touch() };
};

}  // namespace cyd_color_calibration
