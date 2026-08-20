# ESP32-S3 480x480 framebuffer benchmark

Benchmark bezpośredniego renderowania do dwóch framebufferów ST7701S. Każda
klatka jest renderowana do nieaktywnego bufora, prezentowana przez
`present_frame_buffer()` i synchronizowana z VSYNC. Co 60 klatek aplikacja
raportuje rzeczywisty FPS oraz średni czas renderowania pełnego ekranu.

```text
make compile
make upload
make monitor
```

Test wymaga PSRAM i dwóch buforów 480x480 RGB565.
