# JC1060 7-inch display and touch smoke test

Target: ESP32-P4 JC1060 1024x600 panel.

This application will validate the Brick MIPI-DSI display adapter and GT911
touchscreen adapter without ESPHome.

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
