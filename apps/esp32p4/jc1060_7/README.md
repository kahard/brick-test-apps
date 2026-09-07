# JC1060 7-inch demonstrations

These standalone PlatformIO projects follow the structure of the ESP32-S3 4-inch
examples. Each owns its demonstration classes locally; none are moved into BRICK.

| Project | Purpose |
| --- | --- |
| `display_touch_smoke` | Color pattern and touch marker, without LVGL. |
| `display_framebuffer_smoke` | Alternating red/blue frames; buffer fill and display transfer timing. |
| `display_asset_stream_api_framebuffer_smoke` | Four full-screen assets in one BIN; flash/PSRAM/SDMMC, touch mode selection and FPS. |
| `display_lvgl_asset_framebuffer_smoke` | LVGL FULL, touch button and two bundle images (the LVGL/touch counterpart of the S3 demo). |
| `display_lvgl_benchmark_smoke` | Additional P4 benchmark: full-screen colors versus preloaded LVGL images; DMA/VSYNC/scan-out capability check. |
| `sdmmc_filesystem_write_verify` | Existing working SD detection, generated bitmap fonts, touch-triggered write/read verification. |

The additional LVGL benchmark is retained because it covers a different rendering
path from the native asset-storage benchmark. There are six demos rather than five;
there is no additional plain LVGL demo duplicating the existing LVGL/asset/touch one.

## Architecture

- `main.cpp`: calls `Application::initialize()`, then `update()`.
- `ApplicationConfig.h`: compile-time feature selection.
- `Types.h`: board/interface aliases (the previously migrated SD example retains its existing alias location).
- `Application`: owns the board, connects test objects with the required dependencies.
- Small local classes implement rendering, UI, input, asset sources or measurements.
- Timing and logs use BRICK interfaces. ESP-IDF allocation and LVGL adapter setup
  are isolated in supporting classes; there is no FreeRTOS logic in main.

Disabling a peripheral required by a particular demo is an error, not a silent
change of that demo's purpose. The template removes disabled peripheral instances.
CMake compiles the board implementation set; unused code is discarded at link time.
There is no second configuration based on preprocessor feature macros.

## Important hardware difference

The S3 driver exposes its scan-out buffers through `IFrameBufferDisplay`.
The current P4 MIPI driver does not expose that interface. These P4 demos therefore
use two explicitly allocated PSRAM **render buffers** and the existing
`IDisplayDevice` transfer API. The driver owns two additional scan-out buffers.
This preserves the proven display initialization and transfer path; it is not
zero-copy S3-style page flipping.

LVGL uses FULL, not DIRECT. The existing driver enables DMA2D, but its full-frame
path also performs a CPU copy into a scan-out buffer. FPS measures the entire
chosen path, not an isolated DMA2D accelerator or a directly comparable S3 number.

The new native framebuffer and asset demos use 1024x600 RGB565 images.
Two render buffers consume 2,457,600 bytes of PSRAM. The native asset cache adds
4,915,200 bytes when PSRAM storage is first selected. Internal RAM is not used
as an allocation fallback.

## Build and upload

Initialize both public submodules, then run in the desired project:

```text
git submodule update --init --recursive
make compile
make PORT=COM11 upload monitor
```

`JOBS=2` is the default for migrated builds and can be overridden.
CMake descriptors and sdkconfig.defaults are versioned; generated sdkconfig,
PlatformIO build results and asset output remain ignored.

The asset projects regenerate with the framework tools before a build, including
direct `platformio run`. Python with Pillow is required by the image generator.
`make assets` regenerates without compiling.
Output is local to `generated/`; source PNGs belong to each demo.
Upload includes both firmware and the single BIN at the assets partition's
0x610000 offset, using the same selected port.

For native SD asset playback, copy that project's `generated/assets.bin` to
`ASSETS.BIN` in the card root. Do not copy the 4-inch or LVGL bundle.
The demo only reads the card; it does not overwrite its contents.
The separately working SDMMC write-verification application keeps its existing
build commands and font-generation workflow.

## Hardware verification

A successful compilation is not a display or hot-plug test.
After flashing the migrated examples, verify colors/touch, LVGL button response,
asset mode changes and card removal/reinsertion. The working SDMMC pinout,
power-control configuration and lower-level drivers were not changed by this migration.
