# ESP32-S3 ST7701S LVGL framebuffer + GT911 smoke test

Osobny test łączy LVGL w trybie `DIRECT`, dwa frame buffery ST7701S z pixel
clockiem 16 MHz oraz dotyk GT911 jako LVGL pointer input. Na ekranie znajduje
się przycisk `DOTKNIJ` i licznik kliknięć; każde kliknięcie jest również
wypisywane w monitorze szeregowym. Test zawiera również własny asset fontu
Arial 24 px z zakresem Unicode obejmującym polskie znaki.

```powershell
make compile
make upload
make monitor
```
