# CYD: asset z profilem kolorow

Ta aplikacja jest wspolna dla wariantu CYD z PSRAM i bez PSRAM. Uzywa tylko
jednego bufora DMA o wysokosci 16 linii; source asset jest czytany z flasha
ponownie przy kazdym przesunieciu podzialu.

Demonstrator laduje portret z tablica ColorChecker z partycji `assets`. Lewa czesc ekranu przedstawia dane RGB565 bez zmian, prawa wynik zastosowania `Rgb565ColorProfile`. Przesuniecie palcem zmienia polozenie czarnej granicy, a podwojne dotkniecie ustawia ja na srodku.

## Uzycie profilu z kalibratora

1. W `display_color_calibration` zapisz konfiguracje i profil C++.
2. Skopiuj wyeksportowany plik `.h` do tego katalogu jako `src/app/CydColorCalibration.h`, zastepujac obecny profil.
3. Wykonaj `make PORT=COM20 upload`.

Profil identycznosciowy daje dwie identyczne polowy obrazu.

Wariant na PC (`make pc-test` w katalogu glownym) czyta ten sam `assets/generated/assets.bin` zamiast partycji.

## Zrodlo obrazu

Obraz referencyjny pochodzi z materialow testowych [EBU UHDTV](https://tech.ebu.ch/uhdtv): portret, neutralne pola i tablica ColorChecker. Jest tu uzyty wylacznie jako techniczny asset do demonstracji korekcji koloru.
