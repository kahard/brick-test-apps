#pragma once

#include "SdCardTest.h"
#include "Types.h"

namespace cyd_sd_card_filesystem_touch_smoke
{
class Application final
{
public:
    bool initialize();
    void update();

private:
    Board      board_;
    SdCardTest test_{ board_.display(), board_.sd(), board_.touch(), board_.time(), board_.logger() };
};
}  // namespace cyd_sd_card_filesystem_touch_smoke
