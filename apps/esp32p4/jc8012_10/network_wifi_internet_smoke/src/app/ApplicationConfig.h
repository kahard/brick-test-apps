#pragma once

#include <cstddef>
#include <cstdint>

/// One text line is kScreenWidth x kLineHeight pixels; kLineHeight is about
/// 1.4 x the font size (FONT_SIZE = 20 in the Makefile).
inline constexpr std::int32_t kLineHeight  = 28;
inline constexpr std::size_t  kScreenWidth = 800;

/// Hotspot opened when the network cannot be joined or none is configured.
inline constexpr char kHotspotSsid[]     = "BRICK-JC8012";
inline constexpr char kHotspotPassword[] = "brick-setup";
