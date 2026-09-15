#pragma once

#include "ColorProfileAssetTest.h"
#include "Types.h"

namespace cyd_color_profile_asset_smoke
{

class Application final
{
public:
    bool initialize();
    void update();

private:
    Board           board_;
    ColorProfileAssetTest test_{ board_.display(), board_.time(), board_.logger(), board_.touch() };
};

}  // namespace cyd_color_profile_asset_smoke
