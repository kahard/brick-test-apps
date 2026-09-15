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

This is a low-RAM asset test for the ESP-12F and 240x240 ST7789. Two RGB565
images are packed into one bundle and stored as a single `PROGMEM` array. The
generated manifest supplies names, offsets and dimensions; `ProgmemAssetSource`
maps those descriptors to flash. The runtime stripe buffer is only 9,600 bytes;
there is no full-screen framebuffer in RAM.

```text
make assets
make compile
make upload
make monitor
```

GPIO4 is connected to the TTP223/button input. The first image is shown after
startup; each new button press toggles between the two images, while releasing
the button has no effect. The serial monitor reports each toggle. This test
intentionally does not measure FPS because the ESP8266 has no RAM for a
full-screen framebuffer.
