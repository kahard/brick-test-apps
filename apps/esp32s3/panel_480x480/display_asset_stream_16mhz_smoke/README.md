# ESP32-S3 RGB565 asset streaming smoke test

Test odczytu obrazu BIN 480x480 porcjami po 20 wierszy. W pamięci RAM znajduje
się tylko jeden bufor DMA o rozmiarze 480x20x2 bajty; pełny obraz pozostaje w
flash jako osadzony asset.

```powershell
make assets
make compile
make upload
make monitor
```

Log raportuje czas przesłania całej klatki oraz liczbę bajtów. Ten test używa
bezpośrednio `draw_buffer()` i nie korzysta z LVGL.
