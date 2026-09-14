#pragma once

#include "AssetStreamTest.h"
#include "Types.h"

namespace cyd_asset_stream_smoke
{
class Application final
{
public:
    bool initialize();
    void update();

private:
    Board           board_;
    AssetStreamTest test_{ board_ };
};
}  // namespace cyd_asset_stream_smoke
