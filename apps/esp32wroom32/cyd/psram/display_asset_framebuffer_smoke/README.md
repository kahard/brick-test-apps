# CYD asset partition + PSRAM framebuffer smoke test

Test dla wariantu ESP32-2432S028R z PSRAM. Dwa obrazy RGB565 są pakowane do
jednego bundle'a i wgrywane do osobnej partycji `assets`. Po uruchomieniu są
ładowane do dwóch pełnych framebufferów w PSRAM. Dotknięcie ekranu przełącza
aktywny obraz.

Pełny framebuffer ma 153600 bajtów, a dwa bufory zajmują 307200 bajtów PSRAM.
Ponieważ ILI9341 jest podłączony przez SPI, prezentacja nadal wysyła klatkę
przez SPI; wewnętrzny bufor DMA służy wyłącznie do bezpiecznej transmisji.

```text
make compile
make PORT=COM23 upload
make PORT=COM23 monitor
```
