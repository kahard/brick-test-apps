#pragma once

#include "CalibrationTest.h"
#include "Services.h"

namespace cyd_color_calibration
{

/// Portable application; the board is started by the target entry point.
class Application final
{
public:
    explicit Application(const Services& services);

    Status initialize();
    void   update();

private:
    CalibrationTest test_;
};

}  // namespace cyd_color_calibration
