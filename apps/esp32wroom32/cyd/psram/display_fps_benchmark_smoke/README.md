# CYD PSRAM framebuffer FPS benchmark

Benchmark dla wariantu CYD z PSRAM. Program:

- alokuje dwa pełne framebuffer'y 320x240 RGB565 w PSRAM;
- renderuje naprzemiennie czerwony i niebieski ekran;
- mierzy czas renderowania, transferu przez SPI oraz FPS całego cyklu;
- wysyła klatkę do ILI9341 pasami DMA 16 wierszy.

ILI9341 po SPI nie ma sprzętowego page-flip, więc wynik opisuje pełny cykl render + transfer, a nie odświeżanie niezależnego kontrolera scanout.

Uruchomienie:

```text
make PORT=COM17 compile
make PORT=COM17 upload
make PORT=COM17 monitor
```
