# CYD applications

Applications are grouped by the hardware capability they require, rather than
by the board variant used to build them.

## `common`

These applications do not allocate or require PSRAM.  They are built with the
standard ESP32 configuration and may be flashed to either CYD variant:

- `display_touch_smoke` -- display and touch test;
- `sd_card_filesystem_touch_smoke` -- SD-card detection, read/write test and
  generated bitmap fonts.

## `psram`

These applications need PSRAM because they use full-screen framebuffers,
asset caching, or LVGL framebuffers:

- `display_framebuffer_smoke`;
- `display_asset_stream_api_framebuffer_smoke`;
- `display_lvgl_asset_framebuffer_smoke`.

## `no_psram`

These applications exercise mechanisms that need to work within internal RAM:

- `display_asset_stream_api_smoke` -- asset streaming from the flash partition
  through an internal DMA stripe buffer;
- `display_lvgl_asset_smoke` -- LVGL partial-buffer rendering with assets from
  the flash partition.

Do not duplicate an application here when it belongs in `common`.
