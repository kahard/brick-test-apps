#pragma once

#include "brick/interfaces/display/ITouchscreen.h"

class TouchInput final
{
public:
    explicit TouchInput(brick::interfaces::display::ITouchscreen& touch);

    bool pressed();

private:
    brick::interfaces::display::ITouchscreen& touch_;
    bool                                      was_down_ = false;
};
