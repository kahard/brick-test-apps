#pragma once

#include <cstddef>
#include <cstdint>
#include "brick/core/image/AssetBundle.h"

namespace generated_assets {
enum class Id : std::uint32_t {
    joy_tears = 1,
    sweat_smile = 2,
    red_background = 3,
    blue_background = 4,
};

inline constexpr brick::interfaces::display::AssetDescriptor entries[] = {
    {static_cast<std::uint32_t>(Id::joy_tears), 0, 460800, 480, 480, 960, brick::interfaces::display::PixelFormat::rgb565},
    {static_cast<std::uint32_t>(Id::sweat_smile), 460800, 460800, 480, 480, 960, brick::interfaces::display::PixelFormat::rgb565},
    {static_cast<std::uint32_t>(Id::red_background), 921600, 460800, 480, 480, 960, brick::interfaces::display::PixelFormat::rgb565},
    {static_cast<std::uint32_t>(Id::blue_background), 1382400, 460800, 480, 480, 960, brick::interfaces::display::PixelFormat::rgb565},
};
inline constexpr std::size_t entry_count = sizeof(entries) / sizeof(entries[0]);
}  // namespace generated_assets
