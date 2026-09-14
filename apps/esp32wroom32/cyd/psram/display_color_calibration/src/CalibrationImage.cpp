#include "CalibrationImage.h"

#include <cstring>

#include "esp_heap_caps.h"

namespace cyd_color_calibration
{

CalibrationImage::~CalibrationImage()
{
    heap_caps_free(pixels_);
}

bool CalibrationImage::initialize()
{
    pixels_ = static_cast<std::uint8_t*>(heap_caps_malloc(kImageSizeBytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
    return pixels_ != nullptr;
}

bool CalibrationImage::store_stripe(std::uint8_t stripe_index, const std::uint8_t* pixels)
{
    if (pixels_ == nullptr || pixels == nullptr || stripe_index >= stripe_count())
        return false;

    if (stripe_index == 0U)
        received_.fill(false);
    std::memcpy(pixels_ + static_cast<std::size_t>(stripe_index) * stripe_size_bytes(), pixels, stripe_size_bytes());
    received_[stripe_index] = true;
    return true;
}

bool CalibrationImage::complete() const
{
    for (const bool received : received_)
        if (!received)
            return false;
    return pixels_ != nullptr;
}

const std::uint8_t* CalibrationImage::stripe(std::uint8_t stripe_index) const
{
    if (pixels_ == nullptr || stripe_index >= stripe_count())
        return nullptr;
    return pixels_ + static_cast<std::size_t>(stripe_index) * stripe_size_bytes();
}

}  // namespace cyd_color_calibration
