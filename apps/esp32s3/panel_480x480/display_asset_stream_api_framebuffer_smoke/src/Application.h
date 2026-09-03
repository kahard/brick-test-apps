#pragma once

#include "AssetStreamTest.h"

class Application final
{
public:
    bool initialize();
    void update();

private:
    AssetStreamTest::Board board_{ AssetStreamTest::Board::double_buffered_config() };
    AssetStreamTest        test_{ board_ };
};
