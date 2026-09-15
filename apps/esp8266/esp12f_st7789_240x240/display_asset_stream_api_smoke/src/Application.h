#pragma once
#include "AssetStreamerApiTest.h"
namespace esp12f_st7789_asset_stream_api_smoke
{
class Application final
{
public:
    bool initialize();
    void update();

private:
    Board                board_;
    AssetStreamerApiTest test_{ board_.display(), board_.time(), board_.logger() };
};
}  // namespace esp12f_st7789_asset_stream_api_smoke
