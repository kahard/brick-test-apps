#pragma once

#include "ApplicationConfig.h"
#include "SdCardTest.h"
#include "brick/boards/esp32/s3/Panel480Board.h"

class Application final
{
public:
    bool initialize();
    void update();

private:
    brick::boards::Panel480BoardTemplate<SdFeatures> board_;
    SdCardTest                                                    test_{ board_ };
};
