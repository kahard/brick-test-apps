# CYD PSRAM asset-stream benchmark

Test przełącza obrazy `joy_tears` i `sweat_smile` albo jednolite tła czerwone/
niebieskie oraz porównuje trzy źródła tych samych danych: partycję `assets` we
flashu, kopię pakietu w PSRAM i kartę SD. Dotknięcie ekranu zmienia tryb w
kolejności **flash-smiles → flash-red/blue → PSRAM-smiles → PSRAM-red/blue →
SD-smiles → SD-red/blue**.

Log co 30 klatek raportuje FPS oraz średni czas oddzielnie dla wczytania assetu
do framebuffer'a i dla jego prezentacji przez SPI. Na tym panelu czas prezentacji
ogranicza interfejs ILI9341, dlatego nie należy porównywać samego FPS jako wydajności
nośnika danych.

```powershell
make PORT=COM20 upload monitor
```

Przed testem trybu SD skopiuj wygenerowany plik `assets/generated/assets.bin` do
katalogu głównego karty pod nazwą `ASSETS.BIN`. Brak karty lub pliku powoduje
automatyczny powrót do trybu flash.
