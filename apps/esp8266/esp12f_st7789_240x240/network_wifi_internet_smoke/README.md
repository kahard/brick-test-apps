# ESP-12F Wi-Fi and internet test

Wi-Fi of the ESP8266 (ESP8266WiFi for the radio, raw lwIP for DNS and TCP) on the 240x240 ST7789 board; the touch button scans again.

The test itself is shared by every board: see
[apps/shared/network](../../../shared/network/README.md) for what it does, how to compile in
the Wi-Fi network to join and what a pass looks like.

```text
make compile
make upload PORT=COMx
make monitor PORT=COMx
```

Credentials come from `BRICK_WIFI_SSID` / `BRICK_WIFI_PASSWORD` or from
`wifi-credentials.txt` in the repository root; without them only the hotspot
`BRICK-ESP12F` (password `brick-setup`) runs.

PC simulator: `make sim` (SDL window) or `make sim-web` (web browser,
http://localhost:8080/). It joins the simulated "BRICK-LAB";
`SIM_ARGS="--wifi SSID:PASS:RSSI"` changes the networks in range,
`SIM_ARGS=--offline` cuts the internet.
