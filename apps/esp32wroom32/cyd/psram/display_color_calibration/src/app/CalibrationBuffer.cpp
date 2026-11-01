#include "CalibrationBuffer.h"

namespace cyd_color_calibration
{

Status CalibrationBuffer::initialize()
{
    return block_.allocate(kPixelCount * sizeof(std::uint16_t), brick::interfaces::memory::MemoryKind::dma);
}

std::uint16_t* CalibrationBuffer::pixels() const
{
    return reinterpret_cast<std::uint16_t*>(block_.data());
}

}  // namespace cyd_color_calibration
