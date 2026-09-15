# ESP-12F ST7789/button smoke test

Minimal test for the weather-station hardware:

- ESP-12F / ESP8266,
- ST7789V 240x240 SPI display,
- TTP223 or equivalent digital button output on GPIO4.

The test draws six RGB565 color stripes and reports button press/release
edges over the serial monitor. The bottom status stripe changes color while
GPIO4 is active.

The application uses the Brick ESP8266 adapters from `libs/brick`.

Verified on hardware: the ST7789 display works and button state changes are
reported. PlatformIO commands use the repository wrapper to avoid inherited
Git Bash/MSYS variables during tool installation.
# ESP-12F ST7789 SPI frequency benchmark

This is a separate low-RAM benchmark for the ESP-12F and 240x240 ST7789.
Two RGB565 images are stored in flash and are sent in 40-row stripes. The
variants differ only in SPI clock frequency.

Available variants:

- `esp12f_st7789_spi_benchmark_40mhz`
- `esp12f_st7789_spi_benchmark_60mhz`
- `esp12f_st7789_spi_benchmark_80mhz`

The benchmark uses the same image assets and SPI configuration for each
variant, so the reported FPS can be compared directly.

```text
make assets
make compile-all
make upload
make monitor
```

The benchmark reports the number of frames, elapsed time, and effective FPS.
