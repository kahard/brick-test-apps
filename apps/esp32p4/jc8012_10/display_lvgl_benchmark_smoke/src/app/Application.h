#pragma once
#include "LvglTest.h"
#include "Types.h"

class Application final
{
public:
    bool initialize();
    void update();

private:
    Board    board_;
    LvglTest test_{ board_.display(), board_.time(), board_.logger() };
};
