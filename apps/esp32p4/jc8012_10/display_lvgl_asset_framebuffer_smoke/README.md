# JC8012 LVGL asset / framebuffer / touch test

The equivalent of the 4-inch LVGL/touch demo, retaining the existing 7-inch asset
example as well. A button alternates two 100x100 smile images and updates a click
counter. Images are loaded from a single flash bundle into PSRAM during startup.

Application owns a template-configured Board and LvglTest. LvglTest composes:

- LvglRuntime: FULL mode, two PSRAM render buffers and actual elapsed LVGL ticks.
- LvglAssets: generated enum IDs, descriptors and framework PartitionAssetSource.
- TouchView: LVGL widgets and the button callback.
- The framework's LvglTouchAdapter, receiving ITouchscreen.

No direct partition, GPIO or FreeRTOS calls remain in main.cpp.

```text
make compile
make PORT=COM11 upload monitor
```

Every build regenerates generated/assets.bin and generated/generated_assets.h
with the framework scripts. Upload writes both firmware and the asset partition
on the selected port. See [the demo guide](../README.md) for P4/S3 differences.
