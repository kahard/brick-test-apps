#include "CalibrationBuffer.h"

#include "esp_heap_caps.h"

namespace cyd_color_calibration
{

CalibrationBuffer::~CalibrationBuffer()
{
    heap_caps_free(pixels_);
}

bool CalibrationBuffer::initialize()
{
    pixels_ = static_cast<std::uint16_t*>(
        heap_caps_malloc(kPixelCount * sizeof(std::uint16_t), MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL));
    return pixels_ != nullptr;
}

std::uint16_t* CalibrationBuffer::pixels() const
{
    return pixels_;
}

}  // namespace cyd_color_calibration
