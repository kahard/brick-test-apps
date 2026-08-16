# JC1060 7-inch display and touch smoke test

Target: ESP32-P4 JC1060 1024x600 panel.

This application will validate the Brick MIPI-DSI display adapter and GT911
touchscreen adapter without ESPHome.

Brick headers are resolved from the repository submodule by a PlatformIO
pre-script, so the project remains portable when cloned elsewhere.

The application source and Brick API compile on ESP32-P4. The current
PlatformIO `esp32-p4-evboard` profile still fails at final linking because its
generated ESP-IDF memory layout does not declare `sram_low`/`sram_high`; this is
a board profile/toolchain issue, not a display or touchscreen API error.

## Hardware profile

- display: 1024x600 MIPI-DSI, 2 lanes, 600 Mbps, 40 MHz pixel clock;
- display reset: GPIO27;
- GT911 SDA: GPIO7;
- GT911 SCL: GPIO8;
- GT911 default address: `0x5D`;
- ESP32-P4 board definition: `esp32-p4-evboard`.

## Test behavior

- initialize the display and show a color test pattern;
- initialize GT911 and print touch coordinates and state to the serial log;
- draw a marker at the last reported touch position.
