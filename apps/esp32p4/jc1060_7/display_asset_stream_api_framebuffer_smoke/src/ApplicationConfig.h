#pragma once
struct Features
{
    static constexpr bool display   = true;
    static constexpr bool touch     = true;
    static constexpr bool backlight = true;
    static constexpr bool sdmmc     = true;
    static constexpr bool logging   = true;
    static constexpr int  log_level = 0;
};
static_assert(Features::display && Features::backlight, "This demo requires the display and backlight.");
static_assert(Features::touch, "This demo requires touch.");
static_assert(Features::sdmmc, "This demo compares flash, PSRAM and SDMMC.");
