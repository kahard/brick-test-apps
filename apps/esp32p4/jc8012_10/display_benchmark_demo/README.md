# ESP32-P4 10-inch benchmark demo

The shared LVGL benchmark demo ([apps/shared/benchmark](../../../shared/benchmark/README.md))
on the Guition JC8012 board: 800x1280 MIPI DSI panel (portrait) with GSL3680
touch. Six drawing scenes, frames per second, time per frame and CPU load in
the bar at the top, a table after each round and one log line per scene, in
the same format as on every other board.

LVGL draws into two 80-row buffers in internal RAM (fast to draw in); the
PPA copies each area into the panel's scan-out buffer off the screen and the
frame appears with one page flip at a refresh, without tearing
(`LvglDisplayAdapter::create_buffered()`). The panel refreshes at 52 Hz
(pixel clock 60 MHz), so 52 fps is the most a scene can reach. Fills and
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
- The touch controller loads its firmware over I2C first: the demo starts
  about 5 s after the reset.
- The log has one line per scene and one per round:

```text
Benchmark demo board=ESP32-P4 10" 800x1280 MIPI DSI screen=800x1280 mode=RAM buffers + page flip
round=1 scene=Images frames=363 fps=48.36 frame_ms=20.67 cpu=79%
...
round=1 board=ESP32-P4 10" 800x1280 MIPI DSI mean_fps=29.90 mean_cpu=69% lvgl_memory_used=15%
```

## Results (2026-11, 360 MHz, flash QIO, L2 cache 256 KB / 128-byte lines, `-O2`)

| Scene | RAM + page flip (default) | DIRECT |
|---|---|---|
| Widgets | 52.0 fps, CPU 80% | 26.0 fps, CPU 74% |
| Text | 17.4 fps, CPU 62% | 26.0 fps, CPU 93% |
| Images | 48.4 fps, CPU 79% | 26.0 fps, CPU 56% |
| Rectangles | 25.0 fps, CPU 67% | 23.2 fps, CPU 86% |
| Full screen | 26.0 fps, CPU 47% | 26.0 fps, CPU 56% |
| Mixed | 10.8 fps, CPU 83% | 10.4 fps, CPU 82% |
| Mean | 30.0 fps | 22.9 fps |

`kDrawMode` in `src/app/ApplicationConfig.h` chooses the mode:

- `ram_and_flip` (default): drawing in internal RAM fits Widgets and Images
  in one refresh period (19.2 ms). Before a frame the adapter copies into the
  off-screen buffer what the previous frame changed and this one does not
  redraw. Text is slower: its many labels make LVGL redraw the whole screen
  (about 40 ms, so every third refresh).
- `frame_buffers` (DIRECT): LVGL draws straight into the scan-out buffers in
  PSRAM. Most scenes need a little more than one refresh period, so they show
  every second refresh (38.5 ms).
- `partial`: the same RAM buffers copied into the buffer on the screen: no
  refresh wait, but moving objects tear.

`kPartialRows` in `src/target/main.cpp` sets the buffer rows; 40 rows double
the PPA copies and lose fps.

`make sim` (SDL window) or `make sim-web` (web browser) runs the same demo in
the PC simulator; its numbers measure the PC, not the board.
