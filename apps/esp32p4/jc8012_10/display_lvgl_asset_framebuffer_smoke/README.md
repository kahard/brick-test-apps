# JC8012 10-inch LVGL framebuffer smoke test

Test aplikacji BRICK dla panelu JC8012 800x1280 na ESP32-P4.

## Zakres

- pełny framebuffer RGB565 w PSRAM,
- dwa framebuffer'y MIPI-DSI z page flip,
- assety RGB565 z partycji `assets`, ładowane do PSRAM przed prezentacją,
- LVGL 9.5,
- dotyk GSL3680 z przełączaniem assetów przyciskiem,
- rotacje obrazu 0, 90, 180 i 270 stopni,
- benchmark pełnego odświeżania kolorów i assetów.

## Najważniejsze targety

```text
*_r0/r90/r180/r270              test obrazu i assetów
*_touch_r0/...                   test LVGL + dotyku
*_fps_color_r0                   benchmark zmian kolorów
*_fps_asset_r0                   benchmark przełączania assetów
```

Przykłady:

```powershell
make PORT=COM21 upload-touch-90
make PORT=COM21 upload-fps-color
make PORT=COM21 upload-fps-asset
```

## Wynik benchmarku JC8012

Przy pełnym framebufferze 800x1280 uzyskano około `10.4 FPS`, czyli około
`96 ms` na pełne odświeżenie. Wynik był praktycznie taki sam dla zmian
kolorów i assetów z PSRAM, dlatego ograniczeniem jest transfer pełnego
framebuffera przez MIPI-DSI.
