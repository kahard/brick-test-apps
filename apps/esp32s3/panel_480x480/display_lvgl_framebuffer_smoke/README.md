# ESP32-S3 ST7701S LVGL direct framebuffer smoke test

Osobny test LVGL v9 z dwoma pełnymi framebufferami panelu RGB i trybem
`LV_DISPLAY_RENDER_MODE_DIRECT`. LVGL renderuje do nieaktywnego framebuffer'a,
a adapter BRICK przekazuje gotową klatkę do page-flippingu synchronizowanego z
VSYNC.

```powershell
make compile
make upload
make monitor
```

Ten test służy do porównania z `display_lvgl_smoke`, który używa małych buforów
częściowych (`PARTIAL`) i aktualizacji prostokątów.
