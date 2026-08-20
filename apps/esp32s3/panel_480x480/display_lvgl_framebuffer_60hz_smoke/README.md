# ESP32-S3 ST7701S LVGL framebuffer 60 Hz smoke test

Wariant testu `display_lvgl_framebuffer_smoke` z pixel clockiem zwiększonym z
12 MHz do 16 MHz. Przy timingach panelu daje to około 56 Hz zamiast około 42 Hz.
Służy do odróżnienia ograniczenia częstotliwości od problemów page-flippingu.

```powershell
make compile
make upload
make monitor
```
