# JC1060 Ethernet, Wi-Fi and internet test

Ethernet (IP101 on the EMAC of the ESP32-P4) and Wi-Fi through the ESP32-C6 co-processor (`esp_wifi_remote` + `esp_hosted`, see `src/idf_component.yml` and `sdkconfig.defaults`). Ethernet is preferred; pulling the cable moves the traffic to Wi-Fi.

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
`BRICK-JC1060` (password `brick-setup`) runs.

PC simulator: `make sim` (SDL window) or `make sim-web` (web browser,
http://localhost:8080/). It joins the simulated "BRICK-LAB";
`SIM_ARGS="--wifi SSID:PASS:RSSI"` changes the networks in range,
`SIM_ARGS=--offline` cuts the internet.
