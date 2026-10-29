#pragma once

#include "FramebufferTest.h"

class Application final
{
public:
    bool initialize();
    void update();

private:
    using Board = brick::boards::Panel480BoardTemplate<FramebufferFeatures>;

    Board           board_{ Board::double_buffered_config() };
    FramebufferTest test_{ board_ };
};
