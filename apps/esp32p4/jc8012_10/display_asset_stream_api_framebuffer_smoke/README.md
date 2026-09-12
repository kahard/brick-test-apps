# display_asset_stream_api_framebuffer_smoke

See [the JC8012 demo guide](../README.md) for architecture, build instructions
and the difference between P4 render buffers and S3 scan-out buffers.

Touch cycles through six states:

1. Flash / smiles (startup).
2. Flash / red-blue.
3. PSRAM / smiles (all four assets cached on first entry).
4. PSRAM / red-blue.
5. SDMMC / smiles.
6. SDMMC / red-blue, then back to Flash / smiles.

FPS is reported every 60 completed frames; measurements restart on mode changes.
No per-frame UART logging. Missing/short/wrong-size ASSETS.BIN or a read failure
returns playback to flash. Reinserting a card does not switch automatically:
select SDMMC again by touch. Copy this demo's generated/assets.bin to the card
as ASSETS.BIN. No card writes are performed.

Classes: Application -> AssetStreamTest -> AssetPlayback + TouchInput +
AssetBenchmark; AssetPlayback uses AssetSources and FramebufferPresenter.
FrameBuffers and PixelMemory isolate display submission and PSRAM ownership.

Flash playback requires the display to keep refreshing during partition reads.
This demo enables `CONFIG_SPIRAM_XIP_FROM_PSRAM` for application code/constants
and `CONFIG_LCD_DSI_ISR_CACHE_SAFE` for DSI frame interrupts. The separate asset
partition is still read from flash; these options do not cache the asset bundle.
They leave the LCD and SDMMC clocks unchanged. XIP uses additional PSRAM and can
affect measured FPS, so compare storage modes using the same firmware build.

When updating an existing build, enable both options in the project's existing
`sdkconfig.jc8012_10_display_asset_stream_api_framebuffer_smoke` as well; changing
`sdkconfig.defaults` alone does not override saved configuration values.
Use `make clean compile` when switching XIP on an existing build. The local
build hook also tracks `sdkconfig.h` as a linker-script dependency, preventing
reuse of a flash memory layout with PSRAM-XIP startup code.
