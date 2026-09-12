#pragma once

#include "brick/interfaces/display/DisplayTypes.h"

#ifndef BRICK_PANEL_ROTATION
#    define BRICK_PANEL_ROTATION 0
#endif

#if BRICK_PANEL_ROTATION == 0
constexpr brick::interfaces::display::Rotation kRotation = brick::interfaces::display::Rotation::rotate_0;
constexpr std::uint16_t                        kWidth    = 800;
constexpr std::uint16_t                        kHeight   = 1280;
#elif BRICK_PANEL_ROTATION == 90
constexpr brick::interfaces::display::Rotation kRotation = brick::interfaces::display::Rotation::rotate_90;
constexpr std::uint16_t                        kWidth    = 1280;
constexpr std::uint16_t                        kHeight   = 800;
#elif BRICK_PANEL_ROTATION == 180
constexpr brick::interfaces::display::Rotation kRotation = brick::interfaces::display::Rotation::rotate_180;
constexpr std::uint16_t                        kWidth    = 800;
constexpr std::uint16_t                        kHeight   = 1280;
#elif BRICK_PANEL_ROTATION == 270
constexpr brick::interfaces::display::Rotation kRotation = brick::interfaces::display::Rotation::rotate_270;
constexpr std::uint16_t                        kWidth    = 1280;
constexpr std::uint16_t                        kHeight   = 800;
#else
#    error "BRICK_PANEL_ROTATION must be 0, 90, 180 or 270"
#endif

struct Features
{
    static constexpr bool display   = true;
    static constexpr bool touch     = true;
    static constexpr bool backlight = true;
    static constexpr bool sdmmc     = false;
    static constexpr bool logging   = true;
    static constexpr int  log_level = 0;
};

static_assert(Features::display && Features::touch, "This demo requires display and touch.");
