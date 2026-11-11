# ESP32-C6 co-processor update (ESP32-P4 panels)

The Wi-Fi of the Guition ESP32-P4 panels (JC1060 7", JC8012 10") is an
ESP32-C6 co-processor running esp_hosted, reached over SDIO. As shipped the
C6 runs an old esp_hosted (2.1.x on a JC8012, 2.3.x on a JC1060) while the
applications use the 2.12 host component: the C6 then ignores the hotspot
settings (2.1: an open `ESP_xxxxxx` hotspot instead of `BRICK-...`), cannot
read scan results one by one and warns about the version mismatch. This
application updates the C6 to the esp_hosted version of the host, without
any extra hardware: the P4 sends the new firmware over SDIO
(`esp_hosted_slave_ota_*`), the C6 writes it into its other OTA slot and
keeps the old one until the new one is complete.

## Steps (Windows, PowerShell for step 2)

```text
make compile                                         # the P4 application; downloads esp_hosted
powershell -ExecutionPolicy Bypass -File build_coprocessor_firmware.ps1
make upload PORT=COMx                                # the P4 application
make image PORT=COMx                                 # the C6 firmware into the coprocessor_fw partition
make monitor PORT=COMx                               # reset the board: the update runs
```

`build_coprocessor_firmware.ps1` builds `managed_components/espressif__esp_hosted/slave`
for `esp32c6` with the ESP-IDF that PlatformIO installed (`idf.py`; its
Python packages go into `.pio\coprocessor\venv` once). The defaults of that
project match the panels: SDIO with the C6's fixed pins, as on Espressif's
ESP32-P4 Function EV Board.

After `make upload` the application starts at once; without an image it
only logs the versions. With the image in place a reset runs the update:

```text
I (3482) coprocessor_ota: Co-processor firmware 2.1.10, host 2.12.1
I (3482) coprocessor_ota: Sending 1221584 bytes
I (15462) coprocessor_ota: RESULT: firmware sent; restarting to resync with the co-processor
...
I (3484) coprocessor_ota: Co-processor firmware 2.12.1, host 2.12.1
I (3484) coprocessor_ota: RESULT: the co-processor matches the host, nothing to do
```

Then flash the application you want (e.g. the network test) again.

## Results (2026-11)

| Board | Before | After | Transfer |
|---|---|---|---|
| JC8012 10" | 2.1.10 | 2.12.1 | 12 s |
| JC1060 7" | 2.3.2 | 2.12.1 | 11 s |

If the transfer stops, the C6 keeps its old firmware: run it again. A C6
whose new firmware does not answer on SDIO can only be recovered over its
own UART (not tried).
