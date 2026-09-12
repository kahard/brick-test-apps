#pragma once

#include "SdmmcTest.h"

class Application final
{
public:
    bool initialize();
    void update();

private:
    Board     board_;
    SdmmcTest test_{ board_ };
};
