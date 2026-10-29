#pragma once

#include <memory>

#include "ApplicationConfig.h"
#include "brick/boards/esp32/s3/Panel480Board.h"

class AssetStreamTest final
{
public:
    using Board = brick::boards::Panel480BoardTemplate<AssetFeatures>;

    explicit AssetStreamTest(Board& board);
    ~AssetStreamTest();
    bool initialize();
    void update();

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
