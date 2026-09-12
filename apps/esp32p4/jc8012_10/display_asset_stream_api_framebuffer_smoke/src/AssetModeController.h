#pragma once

#include <cstdint>

class AssetModeController final
{
public:
    void         advance() { state_ = static_cast<std::uint8_t>((state_ + 1U) % 6U); }
    void         fallback_to_flash() { state_ = backgrounds() ? 1U : 0U; }
    std::uint8_t storage() const { return state_ / 2U; }
    bool         backgrounds() const { return (state_ & 1U) != 0U; }

private:
    std::uint8_t state_ = 0;
};
