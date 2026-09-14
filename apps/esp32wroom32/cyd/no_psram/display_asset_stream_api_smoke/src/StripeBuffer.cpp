#include "StripeBuffer.h"

#include "esp_heap_caps.h"

namespace cyd_asset_stream_smoke
{
StripeBuffer::~StripeBuffer()
{
    heap_caps_free(data_);
}

bool StripeBuffer::initialize()
{
    data_ = static_cast<std::uint8_t*>(heap_caps_malloc(kBytes, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL));
    return data_ != nullptr;
}

std::uint8_t* StripeBuffer::data() const
{
    return data_;
}

std::size_t StripeBuffer::size() const
{
    return kBytes;
}
}  // namespace cyd_asset_stream_smoke
