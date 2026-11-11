# Benchmark demo (shared)

The portable part of the `display_benchmark_demo` applications: an LVGL
demo that runs a set of drawing scenes, shows how fast the board draws them
and ends every round with a table. Every board has its own small application
(`src/app/Application.h`, `src/target/main.cpp`, `pc/`) around this code, so
the scenes, the measurement and the log format are the same everywhere and
results of different boards compare directly.

Only BRICK interfaces, BRICK core, the BRICK LVGL adapters and LVGL are used:
the same code runs on every board with LVGL and in the PC simulator.

## What it does

The screen has a bar at the top and the scene below it. The scenes run one
after another, 8 s each by default; a touch skips to the next one.

| Scene | What is drawn | What it loads |
|---|---|---|
| Widgets | two arcs, a spinner, three bars, a slider, a scrolling line chart | many small areas |
| Text | a grid of counters whose digits all change, a large clock | glyph rendering |
| Images | ten balls with a soft transparent edge bouncing around | image copy with alpha blending |
| Rectangles | sixteen bouncing rectangles, some rounded, some half transparent | fills and blending |
| Full screen | a gradient over the whole area, its colours change every frame | every pixel, every frame |
| Mixed | Widgets, Text, Images and Rectangles at once, one per quarter | a busy real screen |

The bar shows, over the last half second: frames per second, the average time
from one frame to the next and the CPU load. After the last scene a table
lists every scene and the mean; a touch (or the timeout) starts the next round.

LVGL draws as fast as the display takes frames: its refresh timer runs every
millisecond and every update changes the scene. With the panel's frame
buffers (`DrawMode::frame_buffers`, LVGL DIRECT, or `ram_and_flip`, internal
RAM copied into the buffer off the screen) a frame appears at a refresh of
the panel, so a scene that needs a little more than one refresh period gets
every second one.

## The numbers

- **fps** - frames LVGL rendered per second (`LV_EVENT_RENDER_READY`).
- **ms** - average time from one frame to the next.
- **CPU** - the share of the time the application worked. Waits for the
  display (DMA transfer, VSYNC) do not count: LVGL reports them with
  `LV_EVENT_FLUSH_WAIT_START` / `FINISH`, and the drivers sleep meanwhile. The
  idle time is what the target loop sleeps between updates. A board needs no
  platform API for this.
- The first half second of a scene (creating it, the first full redraw) is
  not measured; a scene skipped before that has no result.

Each scene and each round is logged in one line, the same on every board:

```text
round=1 scene=Images frames=317 fps=42.23 frame_ms=23.67 cpu=87%
round=1 board=ESP32-S3 4" 480x480 RGB mean_fps=26.52 mean_cpu=81% lvgl_memory_used=15%
```

## Files

| File | Role |
|---|---|
| `BenchmarkServices.h` | `Services` (what the demo needs from a board) and `Settings` (board name, timing, LVGL buffers) |
| `BenchmarkDemo.h/.cpp` | LVGL set-up, the round of scenes, touch, log |
| `IScene.h` | a scene: `create(parent)`, `step(elapsed_ms)`, `destroy()`; it fills whatever container it gets |
| `WidgetsScene`, `TextScene`, `ImagesScene`, `RectanglesScene`, `FullScreenScene`, `MixedScene` | the scenes |
| `SpriteImage.h/.cpp` | the ball image, drawn in `initialize()` (no asset files) |
| `FrameStats.h/.cpp` | frames, waits and load |
| `Hud.h/.cpp`, `SummaryView.h/.cpp` | the bar at the top and the table |
| `SceneTools.h/.cpp` | plain boxes and labels, fonts, deterministic random numbers, bouncing motion |

The scenes size themselves from their container, so they also run in a
quarter of the screen (Mixed) and on other screen sizes; the random numbers
are seeded, so every run draws the same scenes.

## Adding a board

1. A new application `apps/<family>/<board>/display_benchmark_demo` with
   `src/app/Application.h` and `Services.h` like the 4-inch one, and an
   `ApplicationConfig.h` with the board's `Features`, timing and LVGL mode.
2. `src/target/main.cpp` fills `Services`: the display, its frame buffers
   (`IFrameBufferDisplay*`, or `nullptr` for PARTIAL buffers on a board
   without enough RAM for two frames), the touch screen (or `nullptr`), memory,
   time and logger; `Settings` names the board and the `DrawMode`. For
   `ram_and_flip` and `partial` (and without frame buffers),
   `partial_rows` and `partial_memory` choose the two LVGL draw buffers.
3. The build adds the shared sources and include directory: a `file(GLOB)`
   of this directory in `src/CMakeLists.txt`, the directory in
   `extra_script.py`, `SOURCES` and `target_include_directories` in
   `pc/simulator.cmake`.
4. `platformio.ini` enables Montserrat 20 (`-DLV_FONT_MONTSERRAT_20=1`; 14 is
   on by default).
5. The PC smoke test touches through the scenes so its screenshot shows the
   summary table.
