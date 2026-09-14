#pragma once

#include "FrameBuffers.h"
#include "brick/interfaces/display/IDisplayDevice.h"

namespace cyd_asset_stream_smoke
{
class FramebufferPresenter final
{
public:
    FramebufferPresenter(brick::interfaces::display::IDisplayDevice& display, FrameBuffers& buffers);
    bool present();

private:
    brick::interfaces::display::IDisplayDevice& display_;
    FrameBuffers&                               buffers_;
};
}  // namespace cyd_asset_stream_smoke
