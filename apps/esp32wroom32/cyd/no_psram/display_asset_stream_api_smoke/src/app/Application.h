#pragma once

#include "AssetStreamTest.h"
#include "Services.h"

namespace cyd_asset_stream_smoke
{
/// Portable application; the board is started by the target entry point.
class Application final
{
public:
    explicit Application(const Services& services);

    Status initialize();
    void   update();

private:
    AssetStreamTest test_;
};
}  // namespace cyd_asset_stream_smoke
