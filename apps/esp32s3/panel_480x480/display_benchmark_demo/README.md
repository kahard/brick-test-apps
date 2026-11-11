# ESP32-S3 4-inch benchmark demo

The shared LVGL benchmark demo ([apps/shared/benchmark](../../../shared/benchmark/README.md))
on the 480x480 ST7701S RGB panel with GT911 touch: six drawing scenes, frames
per second, time per frame and CPU load in the bar at the top, a table after
each round and one log line per scene.

LVGL draws straight into the panel's two scan-out buffers in PSRAM (DIRECT
mode): every frame appears at a refresh, without tearing. The panel refreshes
at 42.3 Hz (pixel clock 12 MHz), so 42 fps is the most a scene can reach.

```text
make compile
make upload
make monitor
```

## What to expect

- The bar shows `1/6 Widgets`, then `Text`, `Images`, `Rectangles`,
  `Full screen` and `Mixed`, 8 s each, with fps, ms and CPU on the right.
- A touch anywhere skips to the next scene.
- After the sixth scene a table lists every scene and the mean for 15 s, then
  the next round starts (a touch starts it at once).
- The log has one line per scene and one per round:

```text
Benchmark demo board=ESP32-S3 4" 480x480 RGB screen=480x480 mode=DIRECT frame buffers
round=1 scene=Widgets frames=159 fps=21.13 frame_ms=47.32 cpu=58%
...
round=1 board=ESP32-S3 4" 480x480 RGB mean_fps=25.56 mean_cpu=78% lvgl_memory_used=15%
```

## Results (2026-11, 240 MHz, flash QIO, 64 KB data cache, `-O2`)

| Scene | DIRECT (default) | RAM + page flip | PARTIAL |
|---|---|---|---|
| Widgets | 21.1 fps, CPU 58% | 24.6 fps, CPU 63% | 40.1 fps |
| Text | 13.1 fps, CPU 96% | 10.7 fps, CPU 79% | 13.2 fps |
| Images | 42.0 fps, CPU 86% | 42.1 fps, CPU 87% | 48.6 fps |
| Rectangles | 24.6 fps, CPU 71% | 26.8 fps, CPU 76% | 36.9 fps |
| Full screen | 42.3 fps, CPU 61% | 36.4 fps, CPU 84% | 40.8 fps |
| Mixed | 10.3 fps, CPU 99% | 9.5 fps, CPU 89% | 10.4 fps |
| Mean | 25.6 fps | 25.0 fps | 31.7 fps |

In DIRECT mode a frame that takes a little longer than one refresh period
(23.6 ms) waits for the next one, which halves Widgets and Rectangles; a flip
that comes just before a VSYNC waits for one more (the DMA has already read
past the end of the old buffer and sends it once more). Rectangles needs
about one refresh period per frame, so its frames come one, two or (about 5%
of them, those that end in that late window) three refreshes apart: the
motion is not even.
`kDrawMode = benchmark_demo::DrawMode::partial` in
`src/app/ApplicationConfig.h` switches to PARTIAL mode: LVGL draws into two
internal-RAM buffers and the driver copies each area into the buffer on the
screen. It is faster in those scenes, but moving objects tear. Text and
Mixed cost the same in both modes: drawing the glyphs takes the CPU's time.
`ram_and_flip` (internal RAM with a page flip, no tearing) gains in Widgets
and Rectangles (no three-refresh frames there), but the ST7701S copies the
areas into the frame buffer by CPU, which costs Text, Full screen and Mixed
more: no gain on average, so DIRECT stays the default.

`make sim` (SDL window) or `make sim-web` (web browser) runs the same demo in
the PC simulator; its numbers measure the PC, not the board.
