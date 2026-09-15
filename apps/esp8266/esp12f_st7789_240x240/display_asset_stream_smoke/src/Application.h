#pragma once
#include "AssetStreamTest.h"
namespace esp12f_st7789_asset_stream_smoke
{
class Application final
{
public:
    bool initialize();
    void update();

private:
    Board           board_;
    AssetStreamTest test_{ board_.display(), board_.button(), board_.time(), board_.logger() };
};
}  // namespace esp12f_st7789_asset_stream_smoke
