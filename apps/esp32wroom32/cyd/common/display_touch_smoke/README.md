# CYD display and touch smoke test

Draws six colour bars and a cross at every touch; each touch is also logged
with its raw coordinates and pressure.

This is the reference layout for portable test applications (see
`libs/brick/docs/application-architecture.md`):

```text
src/app/      portable application: only BRICK interfaces and core
  Services.h    what the application needs from a board
  Application   initialize() / update()
  TouchTest, ColorPattern, TouchMarker
src/target/   CYD entry point (app_main) and board features
pc/           PC simulator entry point and its CTest registration
```

## On the CYD

```text
make compile
make PORT=COM5 upload monitor
```

## In the PC simulator

From the repository root:

```text
make pc
python tools/pc.py run cyd_display_touch_smoke            # window, mouse = touch
python tools/pc.py run cyd_display_touch_smoke --headless --frames 60 \
    --manual-time --touch 160,120@100+400 --screenshot touch.ppm
make pc-test                                             # headless smoke tests
```
