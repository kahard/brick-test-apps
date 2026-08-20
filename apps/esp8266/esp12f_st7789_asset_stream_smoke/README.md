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
# ESP-12F ST7789 asset streaming smoke test

This is a separate low-RAM streaming test for the ESP-12F and 240x240 ST7789.
Two RGB565 images are stored as `const` assets in flash and are sent to the
display in 20-row stripes. The runtime stripe buffer is only 9,600 bytes;
there is no full-screen framebuffer in RAM.

```text
make assets
make compile
make upload
make monitor
```

The benchmark reports the number of frames, elapsed time, and effective FPS.
