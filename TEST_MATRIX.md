# Macierz testów sprzętowych

`make build-all` wykonuje test kompilacyjny. Test sprzętowy oznacza osobne wgranie aplikacji i potwierdzenie obrazu, dotyku oraz logów na właściwej płytce.

| Sprzęt | Podstawowe | Assety | LVGL | Framebuffer / DMA / FPS | Sieć | Dźwięk | Ograniczenia |
|---|---|---|---|---|---|---|---|
| ESP8266 + ST7789 | button smoke | streaming, API | — | SPI benchmark | Wi-Fi + hotspot + internet | — | brak RAM na pełny framebuffer i brak dotyku |
| CYD bez PSRAM | display/touch smoke z markerem dotyku | stripe streaming | partial-buffer smoke | demo benchmarku LVGL (`common`, PARTIAL) | Wi-Fi + hotspot + internet, strona konfiguracji (`common`) | odtwarzanie przez DAC (`common`, sprawdzone na wersji z PSRAM) | brak PSRAM |
| CYD z PSRAM | display/touch smoke z markerem dotyku | asset framebuffer | LVGL framebuffer | pełny framebuffer, FPS, demo benchmarku (`common`) | jak bez PSRAM (`common`) | odtwarzanie przez DAC GPIO26 (`common`) | zależność od konfiguracji SPI |
| ESP32-S3 + ST7701S 4″ | display/touch smoke | single/double buffer, API, streaming | framebuffer, assety, touch | framebuffer benchmark, 60 Hz, DMA + transfer + VSYNC | Wi-Fi + hotspot + internet | odtwarzanie przez wzmacniacz I2S | wymaga właściwego panelu RGB |
| ESP32-P4 + JC1060 7″ | display/touch smoke | LVGL asset framebuffer | LVGL asset framebuffer | DMA2D + dwa bufory scan-out, asset/color FPS | Ethernet + Wi-Fi (C6) + hotspot + internet, przełączanie łączy | odtwarzanie przez ES8311 + NS4150 | wymaga panelu JC1060 |
| ESP32-P4 + JC8012 10″ | display/touch smoke | LVGL asset/stream | LVGL full framebuffer | asset/color FPS, DMA2D + VSYNC, rotacje 0/90/180/270° | Wi-Fi (C6) + hotspot + internet | odtwarzanie przez ES8311 + NS4150 | pełny framebuffer wymaga PSRAM |

Testy pełnego framebufferu nie są uruchamiane na ESP8266 ani CYD bez PSRAM (demo benchmarku na CYD rysuje w trybie PARTIAL i działa na obu wersjach). Dla 4″ i 7″ benchmark kończy się błędem, jeśli sterownik nie zgłosi DMA, VSYNC lub wymaganej liczby buforów.

## Benchmark demo

Aplikacje `display_benchmark_demo` mają wspólną logikę w
[apps/shared/benchmark](apps/shared/benchmark/README.md): sześć scen LVGL
(widżety, tekst, obrazki z przezroczystością, prostokąty, pełny ekran,
wszystko naraz), na górze fps, czas klatki i obciążenie CPU, po rundzie
tabela, w logu jeden wiersz na scenę w tym samym formacie na każdej płytce.
Są wersje dla ESP32-S3 4″, ESP32-P4 JC1060 7″ i JC8012 10″ oraz CYD (jedna aplikacja w `cyd/common` dla wersji z PSRAM i bez; wyniki w ich README).

Na płytce do sprawdzenia: sceny zmieniają się co 8 s, dotyk przełącza scenę,
obraz bez rwania w trybie DIRECT lub RAM + flip (na CYD, w trybie PARTIAL, ruch może lekko rwać), po szóstej scenie tabela mieści się na ekranie, a log ma wiersze
`round=... scene=...`.

## Testy dźwięku

Aplikacje `audio_smoke` mają wspólną logikę w
[apps/shared/audio](apps/shared/audio/README.md) (tam lista kontrolna): na
starcie gama C-dur, dotyk linii gra ton 440 Hz, gamę albo dźwięk z pliku WAV
wkompilowanego w firmware, zmienia głośność co 10% albo zatrzymuje dźwięk.

Na każdej płytce do sprawdzenia: wszystkie dźwięki słychać bez trzasków
i przesteru w całym zakresie głośności, jedno dotknięcie zmienia głośność
o 10%, po dźwięku głośnik milknie bez pyknięcia, a log ma wiersze
`Played ...: N frames in T ms (about R Hz)` z R bliskim 22050 Hz.

## Testy sieci

Aplikacje `network_*_internet_smoke` mają wspólną logikę w
[apps/shared/network](apps/shared/network/README.md) (tam lista kontrolna).
Sieć do połączenia podaje się przy budowaniu (`BRICK_WIFI_SSID` /
`BRICK_WIFI_PASSWORD` albo `wifi-credentials.txt` w katalogu głównym repo);
bez niej aplikacja od razu uruchamia hotspot `BRICK-<PŁYTKA>` (hasło
`brick-setup`), co jest testem trybu AP.

Na każdej płytce do sprawdzenia: połączenie i `INTERNET OK HTTP 204`,
odświeżenie listy sieci dotykiem (ESP-12F: przyciskiem), ponowne połączenie po
wyłączeniu routera, hotspot po trzech błędach (np. złe hasło) i bez danych
sieci (telefon dostaje adres 192.168.4.x, licznik klientów rośnie). Na JC1060
dodatkowo: z kablem ruch idzie przez Ethernet, po wyjęciu kabla przez Wi-Fi.

Do potwierdzenia przy pierwszym uruchomieniu na ESP32-P4: koprocesor ESP32-C6
odpowiada przez SDIO (log `esp_hosted`), adres MAC stacji nie jest zerowy.
