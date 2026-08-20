# ESP32-S3 AssetStreamer framebuffer test

Test używa wspólnego `brick::core::image::AssetStreamer` do odczytu assetu BIN
480x480 porcjami po 20 wierszy do back-framebuffer’a w PSRAM. Pełna klatka jest
prezentowana dopiero po synchronizacji VSYNC.

```powershell
make assets
make compile
make upload
make monitor
```

Log raportuje czas wczytania assetu do framebuffer’a. Test nie korzysta z LVGL;
sprawdza bezpośrednio ścieżkę asset → framebuffer → page flip.
