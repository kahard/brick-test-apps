#pragma once

struct Features
{
    static constexpr bool display   = true;
    static constexpr bool touch     = true;
    static constexpr bool backlight = true;
    static constexpr bool sd        = true;
    static constexpr bool logging   = true;
    static constexpr int  log_level = 0;
};

static_assert(Features::display && Features::touch && Features::sd, "This demo requires display, touch and SDMMC.");
