# CYD ILI9341/XPT2046 display touch smoke test

Test aplikacji Brick dla ESP32-2432S028R (CYD):

- ILI9341 320x240 po SPI2: SCLK14, MOSI13, MISO12, CS15, DC2;
- podświetlenie GPIO21;
- XPT2046 po SPI3: SCLK25, MOSI32, MISO39, CS33, IRQ36;
- kalibracja i transformacja dotyku zgodna z konfiguracją ESPHome projektu CYD.

Uruchomienie:

```text
make compile
make upload
make monitor
```

Test wyświetla kolorowe pasy i loguje współrzędne dotyku.
