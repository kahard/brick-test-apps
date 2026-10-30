#pragma once

#include "AssetStreamerApiTest.h"
#include "Services.h"

namespace esp12f_st7789_asset_stream_api_smoke
{
/// Portable application; the board is started by the target entry point.
class Application final
{
public:
    explicit Application(const Services& services);

    bool initialize();
    void update();

private:
    AssetStreamerApiTest test_;
};
}  // namespace esp12f_st7789_asset_stream_api_smoke
