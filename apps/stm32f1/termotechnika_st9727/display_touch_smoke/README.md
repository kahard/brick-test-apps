# Termotechnika ST-9727 display/touch smoke test

PlatformIO smoke test for the STM32F105VC reference panel.

It initializes the SSD1963 with the legacy 480x272 timing profile, draws RGB
stripes, reads the FT5x06-compatible capacitive touch controller on I2C1 and
draws a marker for every press/move event.

Build with `platformio run`; upload using an ST-Link with
`platformio run --target upload`. The default `WR` mapping is the ST280/281
variant (PC2). For ST286/595 change `wr_port`/`wr_pin` in a copied profile to
GPIOB/GPIO_PIN_9 before hardware testing.
