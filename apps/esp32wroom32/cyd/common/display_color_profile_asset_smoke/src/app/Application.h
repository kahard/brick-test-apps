#pragma once

#include "ColorProfileAssetTest.h"
#include "Services.h"

namespace cyd_color_profile_asset_smoke
{

/// Portable application; the board is started by the target entry point.
class Application final
{
public:
    explicit Application(const Services& services);

    Status initialize();
    void   update();

private:
    ColorProfileAssetTest test_;
};

}  // namespace cyd_color_profile_asset_smoke
