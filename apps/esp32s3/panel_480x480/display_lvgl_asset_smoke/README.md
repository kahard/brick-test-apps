# ESP32-S3 LVGL asset smoke test

Osobny test ST7701S + GT911 + LVGL DIRECT. Projekt używa dwóch obrazków:

- `C asset` — obrazek Twemoji jako tablica bajtów w wygenerowanym pliku C/H,
- `BIN asset` — obrazek Twemoji jako surowy plik RGB565 dołączony przez linker,
- fontu Roboto wygenerowanego do assetu LVGL,
- przycisku LVGL z licznikiem naciśnięć.

Oba obrazy mają rozmiar 100x100. Przycisk znajduje się poniżej obrazów i
zwiększa licznik `Naciśnięcia`.

```powershell
make assets
make compile
make upload
make monitor
```

Targety assetów można uruchamiać osobno:

```powershell
make font
make image
make image-bin
```

Źródła znajdują się w `assets/`, a wygenerowane pliki C/H w `src/fonts/` oraz
`src/images/`.
Target `image-bin` dodatkowo tworzy surowy strumień pikseli w
`assets/generated/`, przeznaczony do odczytu porcjami z flash lub karty SD.
