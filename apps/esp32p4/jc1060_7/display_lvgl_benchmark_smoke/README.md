# display_lvgl_benchmark_smoke

See [the JC1060 demo guide](../README.md) for architecture, build instructions
and the difference between P4 render buffers and S3 scan-out buffers.

Runs repeated series of 60 frames: preloaded smile images, then full-screen
red/blue. Two-second pauses separate the series. One frame is processed per
update; an ITimeProvider supplies timestamps and LVGL's actual elapsed tick time.
The capability check retains the previous P4 DMA/VSYNC/scan-out diagnostic.

Classes: Application -> LvglTest -> LvglRuntime + LvglAssets + BenchmarkSeries.
This is a rendering benchmark, not a flash/SD throughput benchmark.
