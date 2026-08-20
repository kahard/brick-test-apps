# ESP32-S3 480x480 display/touch smoke test

Minimalny test panelu RGB 480x480 z kontrolerem ST7701S i dotykiem GT911.
Test rysuje sześć kolorowych pasów i wypisuje zdarzenia dotyku w monitorze
szeregowym.

```text
make compile
make upload
make monitor
```

Zweryfikowano sprzętowo: obraz ST7701S 480x480 oraz dotyk GT911 działają.
Polecenia `compile` i `upload` korzystają z wrappera, który izoluje PlatformIO
od zmiennych środowiskowych Git Bash/MSYS.
