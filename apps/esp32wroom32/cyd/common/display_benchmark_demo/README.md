# CYD benchmark demo (ESP32)

The shared LVGL benchmark demo ([apps/shared/benchmark](../../../../shared/benchmark/README.md))
on the CYD (ESP32-2432S028R, with or without PSRAM: the demo uses none):
320x240 ILI9341 over SPI (80 MHz) with XPT2046 touch. Six drawing scenes,
frames per second, time per frame and CPU load in the bar at the top, a
table after each round and one log line per scene, in the same format as on
every other board.

The ILI9341 has no scan-out buffers, so LVGL draws in PARTIAL mode into two
40-row buffers in DMA-capable internal RAM (the ESP32's SPI DMA cannot read
PSRAM, so it stays unused); the SPI DMA sends one while LVGL draws the
other. Moving objects can tear: the panel has no VSYNC to wait for.

```text
make compile
make upload PORT=COMx
make monitor PORT=COMx
```

Flashing this CYD needs download mode by hand: hold BOOT, tap RST, release
BOOT, then upload.

## Results (2026-11, 240 MHz, flash QIO 80 MHz, `-O2`)

| Scene | CYD without PSRAM (ESP32 v3.1) | CYD with PSRAM (ESP32 v1.1, PSRAM build) |
|---|---|---|
| Widgets | 42.7 fps, CPU 97% | 34.4 fps, CPU 97% |
| Text | 28.6 fps, CPU 98% | 28.4 fps, CPU 98% |
| Images | 61.9 fps, CPU 95% | 51.6 fps, CPU 97% |
| Rectangles | 43.7 fps, CPU 97% | 33.8 fps, CPU 97% |
| Full screen | 50.3 fps, CPU 95% | 46.0 fps, CPU 96% |
| Mixed | 13.9 fps, CPU 99% | 12.2 fps, CPU 99% |
| Mean | 40.2 fps | 34.4 fps |

Both in PARTIAL mode with 2 x 40 rows. The second column was built with
PSRAM support (`CONFIG_SPIRAM`): on an ESP32 below revision 3 that adds the
PSRAM cache workaround (`-mfix-esp32-psram-cache-issue`, extra memory
barriers in all code), which is the likely cause of the difference. This
application is now built without PSRAM.

Drawing takes the CPU's time: a full-screen transfer (about 16 ms at
80 MHz) overlaps the drawing of the next stripe, so even Full screen
(21.7 ms per frame) is limited by LVGL, not by the SPI.

```text
Benchmark demo board=ESP32 CYD 2.8" 320x240 SPI screen=320x240 mode=PARTIAL buffers
round=1 scene=Images frames=386 fps=51.55 frame_ms=19.39 cpu=97%
...
round=1 board=ESP32 CYD 2.8" 320x240 SPI mean_fps=34.38 mean_cpu=97% lvgl_memory_used=15%
```

`make sim` (SDL window) or `make sim-web` (web browser) runs the same demo in
the PC simulator; its numbers measure the PC, not the board.
