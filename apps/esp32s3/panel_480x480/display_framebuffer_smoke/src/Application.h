#pragma once

#include "FramebufferTest.h"

class Application final
{
public:
    bool initialize();
    void update();

private:
    using Board = brick::platform::esp32::s3::Panel480BoardTemplate<FramebufferFeatures>;

    Board           board_{ Board::double_buffered_config() };
    FramebufferTest test_{ board_ };
};
