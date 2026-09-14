# CYD: asset z profilem kolorow

Demonstrator laduje portret z tablica ColorChecker z partycji `assets`. Lewa czesc ekranu przedstawia dane RGB565 bez zmian, prawa wynik zastosowania `Rgb565ColorProfile`. Przesuniecie palcem zmienia polozenie czarnej granicy, a podwojne dotkniecie ustawia ja na srodku.

## Uzycie profilu z kalibratora

1. W `display_color_calibration` zapisz konfiguracje i profil C++.
2. Skopiuj wyeksportowany plik `.h` do tego katalogu jako `src/ColorProfile.h`, zastepujac plik domyslny.
3. Wykonaj `make PORT=COM20 upload`.

Domyslny `ColorProfile.h` zwraca profil identycznosciowy, dlatego obie strony obrazu beda wtedy takie same.

## Zrodlo obrazu

Obraz referencyjny pochodzi z materialow testowych [EBU UHDTV](https://tech.ebu.ch/uhdtv): portret, neutralne pola i tablica ColorChecker. Jest tu uzyty wylacznie jako techniczny asset do demonstracji korekcji koloru.
