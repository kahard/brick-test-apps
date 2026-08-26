# Macierz testów sprzętowych

`make build-all` wykonuje test kompilacyjny. Test sprzętowy oznacza osobne wgranie aplikacji i potwierdzenie obrazu, dotyku oraz logów na właściwej płytce.

| Sprzęt | Podstawowe | Assety | LVGL | Framebuffer / DMA / FPS | Ograniczenia |
|---|---|---|---|---|---|
| ESP8266 + ST7789 | button smoke | streaming, API | — | SPI benchmark | brak RAM na pełny framebuffer i brak dotyku |
| CYD bez PSRAM | display/touch smoke z markerem dotyku | stripe streaming | partial-buffer smoke | — | brak PSRAM |
| CYD z PSRAM | display/touch smoke z markerem dotyku | asset framebuffer | LVGL framebuffer | pełny framebuffer, FPS | zależność od konfiguracji SPI |
| ESP32-S3 + ST7701S 4″ | display/touch smoke | single/double buffer, API, streaming | framebuffer, assety, touch | framebuffer benchmark, 60 Hz, DMA + transfer + VSYNC | wymaga właściwego panelu RGB |
| ESP32-P4 + JC1060 7″ | display/touch smoke | LVGL asset framebuffer | LVGL asset framebuffer | DMA2D + dwa bufory scan-out, asset/color FPS | wymaga panelu JC1060 |
| ESP32-P4 + JC8012 10″ | display/touch smoke | LVGL asset/stream | LVGL full framebuffer | asset/color FPS, DMA2D + VSYNC, rotacje 0/90/180/270° | pełny framebuffer wymaga PSRAM |

Testy pełnego framebufferu i FPS nie są uruchamiane na ESP8266 ani CYD bez PSRAM. Dla 4″ i 7″ benchmark kończy się błędem, jeśli sterownik nie zgłosi DMA, VSYNC lub wymaganej liczby buforów.
