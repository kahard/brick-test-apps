#pragma once

#include <cstddef>
#include <cstdint>

namespace cyd_asset_stream_smoke
{
class StripeBuffer final
{
public:
    ~StripeBuffer();
    bool          initialize();
    std::uint8_t* data() const;
    std::size_t   size() const;

    static constexpr std::uint16_t height() { return kHeight; }

private:
    static constexpr std::uint16_t kHeight = 16U;
    static constexpr std::size_t   kBytes  = 320U * kHeight * 2U;
    std::uint8_t*                  data_   = nullptr;
};
}  // namespace cyd_asset_stream_smoke
