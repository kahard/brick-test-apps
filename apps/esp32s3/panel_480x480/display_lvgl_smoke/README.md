# ESP32-S3 ST7701S LVGL smoke test

Minimalny test integracji LVGL 9.5 z adapterem wyświetlacza BRICK.

Test używa trybu `PARTIAL` oraz dwóch buforów renderujących po 40 linii. LVGL
renderuje ekran, a adapter przekazuje obszary zmian do sterownika ST7701S przez
wspólny interfejs BRICK. Jest to osobny test od bezpośredniego page-flip/
framebuffer benchmarku.

```powershell
make compile
make upload
make monitor
```

Oczekiwany obraz to ciemnoniebieskie tło z napisem `BRICK + LVGL`. W monitorze
powinny pojawić się komunikaty o utworzeniu wyświetlacza LVGL i aktywnej pętli
renderującej.
