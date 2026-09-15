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
    AssetStreamTest test_{ board_.display(), board_.sd(), board_.touch(), board_.time(), board_.logger() };
};
}  // namespace cyd_asset_stream_smoke
