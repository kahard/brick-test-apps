# JC1060 7-inch display and touch smoke test

Target: ESP32-P4 JC1060 1024x600 panel.

This application will validate the Brick MIPI-DSI display adapter and GT911
touchscreen adapter without ESPHome.

Brick headers are resolved from the repository submodule by a PlatformIO
pre-script, so the project remains portable when cloned elsewhere.

The application source and Brick API compile on ESP32-P4. The current
The test uses the `esp32-p4_r3-evboard` profile because the older ES profile
generates an incompatible ESP-IDF memory layout. This is a linker/profile
compatibility detail; it does not affect the Brick display or touchscreen API.

## Hardware profile

- display: 1024x600 MIPI-DSI, 2 lanes, 600 Mbps, 40 MHz pixel clock;
- display reset: GPIO27;
- GT911 SDA: GPIO7;
- GT911 SCL: GPIO8;
- GT911 default address: `0x5D`;
- ESP32-P4 board definition: `esp32-p4_r3-evboard`.

## Test behavior

- initialize the display and show a color test pattern;
- initialize GT911 and print touch coordinates and state to the serial log;
- draw a marker at the last reported touch position.

## Build commands

Run these commands from this directory:

```text
make clean    # remove the PlatformIO build output
make compile  # compile the application
make upload   # compile if needed and upload to the board
make monitor  # open the serial monitor
```

The `PIO` and `ENV` variables can be overridden when needed, for example:
`make compile PIO=pio ENV=jc1060_7_display_touch_smoke`.
