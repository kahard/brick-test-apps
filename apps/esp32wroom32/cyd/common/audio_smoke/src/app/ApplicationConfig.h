#pragma once

#include <cstddef>
#include <cstdint>

/// One text line is kScreenWidth x kLineHeight pixels; kLineHeight is about
/// 1.4 x the font size (FONT_SIZE = 16 in the Makefile).
inline constexpr std::int32_t kLineHeight  = 22;
inline constexpr std::size_t  kScreenWidth = 320;

inline constexpr char kBoardName[] = "CYD 2.8\" ESP32";
