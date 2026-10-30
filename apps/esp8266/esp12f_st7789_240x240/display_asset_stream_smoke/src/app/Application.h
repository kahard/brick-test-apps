#pragma once

#include "AssetStreamTest.h"
#include "Services.h"

namespace esp12f_st7789_asset_stream_smoke
{
/// Portable application; the board is started by the target entry point.
class Application final
{
public:
    explicit Application(const Services& services);

    bool initialize();
    void update();

private:
    AssetStreamTest test_;
};
}  // namespace esp12f_st7789_asset_stream_smoke
