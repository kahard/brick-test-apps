#include "CalibrationImage.h"

#include <cstring>

namespace cyd_color_calibration
{

bool CalibrationImage::initialize()
{
    return block_.allocate(kImageSizeBytes, brick::interfaces::memory::MemoryKind::external);
}

bool CalibrationImage::store_stripe(std::uint8_t stripe_index, const std::uint8_t* pixels)
{
    if (!block_ || pixels == nullptr || stripe_index >= stripe_count())
        return false;

    if (stripe_index == 0U)
        received_.fill(false);
    std::memcpy(block_.data() + static_cast<std::size_t>(stripe_index) * stripe_size_bytes(), pixels,
                stripe_size_bytes());
    received_[stripe_index] = true;
    return true;
}

bool CalibrationImage::complete() const
{
    for (const bool received : received_)
        if (!received)
            return false;
    return static_cast<bool>(block_);
}

const std::uint8_t* CalibrationImage::stripe(std::uint8_t stripe_index) const
{
    if (!block_ || stripe_index >= stripe_count())
        return nullptr;
    return block_.data() + static_cast<std::size_t>(stripe_index) * stripe_size_bytes();
}

}  // namespace cyd_color_calibration
