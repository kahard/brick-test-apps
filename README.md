# BRICK test applications

Test applications are grouped by MCU family and board variant:

```text
brick_test_apps/
├── libs/
│   └── brick/         BRICK Framework submodule under test (incl. boards/)
├── apps/
│   ├── esp32p4/
│   │   ├── jc1060_7/
│   │   └── jc8012_10/
│   ├── esp32s3/
│   │   └── panel_480x480/
│   ├── esp32wroom32/
│   │   └── cyd/
│   └── esp8266/
│       └── esp12f_st7789_240x240/
└── README.md
```

Logic shared by the applications of several boards lives in `apps/shared`
(for now the network test, `apps/shared/network`); each board still has its
own application with `src/app`, `src/target` and `pc/`.

Use names in the form:

```text
<board>_<scope>_<purpose>
```

Examples: `display_touch_smoke`, `jpeg_decode_benchmark`,
`audio_output_smoke`.

Each application should contain its own `README.md` describing the hardware,
expected wiring, build/upload commands, and the result of the test.

The [JC1060 7-inch demo guide](apps/esp32p4/jc1060_7/README.md) describes the
Application/board-template structure and maps these demos to the 4-inch examples.

## Checkout and update

Clone the repository together with BRICK:

```text
git clone --recurse-submodules <brick-test-apps-url>
```

For an existing checkout:

```text
git submodule update --init --recursive
```

The submodule is pinned to a specific BRICK commit so test results remain
reproducible. Update it deliberately when a new framework version is ready.
Board packages are supplied by the framework in `libs/brick/boards/include`.

## Development environment

The only host requirements are Git, GNU Make and Python 3.10-3.14 (a native
one; on Windows the MSYS2/Git Bash Python is not supported by ESP-IDF).
Everything else is pinned in the repository and installed into `.venv`:

```text
make setup           # create .venv, install requirements.txt, fetch missing submodules
make setup-packages  # optional: also pre-download all platforms/toolchains
make doctor          # check the environment
```

`make setup` picks a supported Python by itself (on Windows through the `py`
launcher), so it also works when the default interpreter is newer. Run it
again after pulling changes to `requirements.txt`.

Pinned versions:

- `requirements.txt` - PlatformIO Core, Pillow, esptool and clang-format 17,
- `platformio-versions.ini` - PlatformIO platforms and libraries; every
  `platformio.ini` includes it and uses `${versions.<name>}`.

ESP32 apps use the [pioarduino](https://github.com/pioarduino/platform-espressif32)
platform because the official one does not support ESP32-P4. Both register as
`espressif32`, so an unpinned `platform = espressif32` silently picks whichever
is installed locally.

Common commands (run from the repository root):

```text
make list                          # list applications
make build-all                     # build every application
make build-all FILTER=esp32p4      # build only matching applications
make clean-all
make format / make format-check    # clang-format 17 from .venv
make setup-vscode                  # IntelliSense configuration
```

## Portable applications and the PC simulator

Applications are being split into a portable part and per-target entry
points, following `libs/brick/docs/application-architecture.md` and the
embedded coding rules in `libs/brick/docs/embedded-coding-rules.md`:

```text
src/app/     portable code: BRICK interfaces and core only
src/target/  embedded entry point (app_main / setup+loop) and board choice
pc/          PC simulator entry point (main.cpp) and simulator.cmake
```

Applications with a `pc/` directory also build for the desktop:

```text
make pc                                      # build all simulators (CMake)
make pc-test                                 # run them headless in CTest
python tools/pc.py run <name> [options...]   # e.g. cyd_display_touch_smoke
make rules                                   # embedded coding rules report
make rules-check                             # the same, failing on any finding (CI)
```

The simulator opens an SDL2 window when SDL2 is installed (mouse = touch,
space = button); `--headless --frames N --touch X,Y@MS --screenshot F.ppm`
runs it without a window. The reference application is
`apps/esp32wroom32/cyd/common/display_touch_smoke`.

Each application keeps its own `Makefile` (`make compile`, `make upload`,
`make monitor`, `PORT=...`). They include `tools/app.mk`, which runs PlatformIO
and Python from `.venv` through `tools/pio.py`. The tools are plain Python, so
the same commands work on Windows, Linux and macOS.
