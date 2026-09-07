# JC1060 display and touch test

Uses the board template, selected by Features in ApplicationConfig.h and aliased
in Types.h. Application owns the board and injects display, touch, time and logger
interfaces into TouchTest. ColorPattern and TouchMarker handle rendering.

The test draws six horizontal colors, then marks touch positions. Pixel transfers
must complete before reusing the render buffer. The board initializes the panel
and enables its backlight; there is no GPIO setup in the application.

Run from this folder:

```text
make compile
make PORT=COM11 upload monitor
```

The PlatformIO board is esp32-p4-evboard; the JC1060 hardware profile lives in
libs/brick-boards. See [the demo guide](../README.md) for the project overview.
