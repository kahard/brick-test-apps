# JC8012 Wi-Fi and internet test

Wi-Fi of the 10-inch ESP32-P4 panel through the ESP32-C6 co-processor (`esp_wifi_remote` + `esp_hosted`, see `src/idf_component.yml` and `sdkconfig.defaults`), portrait 800x1280.

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
`BRICK-JC8012` (password `brick-setup`) runs.

PC simulator (from the repository root: `make pc`, then run the executable):
it joins the simulated "BRICK-LAB", `--wifi SSID:PASS:RSSI` changes the
networks in range, `--offline` cuts the internet.
