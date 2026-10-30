#include "LvglTest.h"

namespace lvgl_benchmark_smoke
{
LvglTest::LvglTest(const Services& services)
    : display_(services.display), logger_(services.logger), runtime_(services.display, services.memory, services.time),
      assets_(services.assets, services.memory), series_(services.time, services.logger)
{
}

bool LvglTest::initialize()
{
    const brick::interfaces::display::DisplayCapabilities caps = display_.capabilities();
    if (!caps.dma || !caps.vsync || !caps.scanout_buffers || caps.max_buffer_count < 2)
    {
        logger_.error("lvgl_benchmark", "DMA scan-out capability check failed");
        return false;
    }
    logger_.info("lvgl_benchmark", "DMA=%d vsync=%d scanout=%d buffers=%u alignment=%u", caps.dma, caps.vsync,
                 caps.scanout_buffers, static_cast<unsigned>(caps.max_buffer_count),
                 static_cast<unsigned>(caps.dma_alignment_bytes));
    if (!assets_.initialize() || !runtime_.initialize())
        return false;
    series_.initialize(runtime_.display(), assets_);
    return true;
}

void LvglTest::update()
{
    runtime_.update();
    series_.update();
}
}  // namespace lvgl_benchmark_smoke
