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
# ESP-12F ST7789 AssetStreamer color-boundary test

This test uses `brick::core::image::AssetStreamer` for the complete image
streaming loop. The ESP8266-specific reader supplies alternating full-screen
red and blue RGB565 frames from `PROGMEM`.

Available variants:

The stable hardware configuration is 60 MHz SPI and a 40-row stripe.

The sharp red/blue boundary makes tearing easy to see. The stable hardware
configuration is 60 MHz SPI and a 40-row stripe. Since this panel has no
configured TE/VSYNC signal, tearing is expected during full-screen streaming.

```text
make assets
make compile-all
make upload
make monitor
```

The benchmark reports the number of frames, elapsed time, and effective FPS.
