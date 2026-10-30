#pragma once

#include "brick/boards/esp32/p4/Jc1060Board.h"

struct SdFeatures
{
    static constexpr bool display = true;
    static constexpr bool touch = true;
    static constexpr bool backlight = true;
    static constexpr bool sd = true;
    static constexpr bool logging = true;
    static constexpr int log_level = 0;
};

using Board = brick::boards::Jc1060BoardTemplate<SdFeatures>;
