#pragma once

#include "ApplicationConfig.h"
#include "LvglTest.h"
#include "brick/boards/esp32/s3/Panel480BoardTemplate.h"

class Application final
{
public:
    using Board = brick::platform::esp32::s3::Panel480BoardTemplate<LvglFeatures>;

    Application();
    bool initialize();
    void update();

private:
    Board    board_;
    LvglTest test_;
};
