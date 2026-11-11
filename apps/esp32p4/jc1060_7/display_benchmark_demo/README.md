# ESP32-P4 7-inch benchmark demo

The shared LVGL benchmark demo ([apps/shared/benchmark](../../../shared/benchmark/README.md))
on the Guition JC1060 board: 1024x600 MIPI DSI panel with GT911 touch. Six
drawing scenes, frames per second, time per frame and CPU load in the bar at
the top, a table after each round and one log line per scene, in the same
format as on every other board.

LVGL draws into two 48-row buffers in internal RAM (fast to draw in); the
PPA copies each area into the panel's scan-out buffer off the screen and the
frame appears with one page flip at a refresh, without tearing
(`LvglDisplayAdapter::create_buffered()`). The panel refreshes at 59 Hz
(pixel clock 52 MHz), so 59 fps is the most a scene can reach. Fills and
RGB565 images go through LVGL's PPA unit (`LV_USE_PPA`).

```text
make compile
make upload PORT=COMx
make monitor PORT=COMx
```

## What to expect

- The bar shows `1/6 Widgets`, then `Text`, `Images`, `Rectangles`,
  `Full screen` and `Mixed`, 8 s each, with fps, ms and CPU on the right.
- A touch anywhere skips to the next scene.
- After the sixth scene a table lists every scene and the mean for 15 s, then
  the next round starts (a touch starts it at once).
- The log has one line per scene and one per round:

```text
Benchmark demo board=ESP32-P4 7" 1024x600 MIPI DSI screen=1024x600 mode=RAM buffers + page flip
round=1 scene=Images frames=443 fps=59.10 frame_ms=16.91 cpu=65%
...
round=1 board=ESP32-P4 7" 1024x600 MIPI DSI mean_fps=42.79 mean_cpu=72% lvgl_memory_used=15%
```

## Results (2026-11, 360 MHz, flash QIO, L2 cache 256 KB / 128-byte lines, `-O2`)

| Scene | RAM + page flip (default) | DIRECT |
|---|---|---|
| Widgets | 59.1 fps, CPU 77% | 29.7 fps, CPU 55% |
| Text | 29.1 fps, CPU 89% | 29.6 fps, CPU 85% |
| Images | 59.1 fps, CPU 65% | 59.1 fps, CPU 76% |
| Rectangles | 35.6 fps, CPU 61% | 29.5 fps, CPU 63% |
| Full screen | 59.1 fps, CPU 51% | 59.1 fps, CPU 64% |
| Mixed | 14.8 fps, CPU 91% | 14.8 fps, CPU 87% |
| Mean | 42.8 fps | 37.0 fps |

`kDrawMode` in `src/app/ApplicationConfig.h` chooses the mode:

- `ram_and_flip` (default): drawing in internal RAM fits Widgets in one
  refresh period (16.9 ms). Before a frame the adapter copies into the
  off-screen buffer what the previous frame changed and this one does not
  redraw.
- `frame_buffers` (DIRECT): LVGL draws straight into the scan-out buffers in
  PSRAM; Widgets needs a little more than one refresh period, so it shows
  every second refresh (33.8 ms).
- `partial`: the same RAM buffers copied into the buffer on the screen: no
  refresh wait, but moving objects tear.

Rectangles needs about one refresh period per frame in both modes, so its
frames come one, two or (a few percent) three refreshes apart.
`kPartialRows` in `src/target/main.cpp` sets the buffer rows.

`make sim` (SDL window) or `make sim-web` (web browser) runs the same demo in
the PC simulator; its numbers measure the PC, not the board.
