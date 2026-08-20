# ESP32-S3 480x480 framebuffer smoke test

Osobny test dwóch framebufferów RGB dla ST7701S. Aplikacja wypełnia bufor
bezpośrednio, prezentuje go przez `present_frame_buffer()` i czeka na VSYNC.
Kolory czerwony i niebieski są przełączane co około 500 ms. Test mierzy również
czas wypełniania bufora oraz czas od prezentacji do VSYNC.

```text
make compile
make upload
make monitor
```

Ten test wymaga PSRAM i dwóch buforów 480x480 RGB565.
