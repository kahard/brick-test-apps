#pragma once

#include "SdCardTest.h"
#include "Services.h"

namespace cyd_sd_card_filesystem_touch_smoke
{
/// Portable application; the board is started by the target entry point.
class Application final
{
public:
    explicit Application(const Services& services);

    Status initialize();
    void   update();

private:
    SdCardTest test_;
};
}  // namespace cyd_sd_card_filesystem_touch_smoke
