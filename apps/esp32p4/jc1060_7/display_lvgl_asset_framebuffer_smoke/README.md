# JC1060 7" LVGL asset framebuffer smoke test

Testuje pełny framebuffer LVGL dla panelu MIPI DSI JC1060 1024x600 z dotykiem GT911.
Obraz `joy_tears` jest ładowany z partycji `assets`, a przycisk na ekranie zwiększa licznik dotknięć.

## Build i uruchomienie

```powershell
make ENV=jc1060_7_display_lvgl_asset_framebuffer_smoke PORT=COM17 compile
make ENV=jc1060_7_display_lvgl_asset_framebuffer_smoke PORT=COM17 upload
make ENV=jc1060_7_display_lvgl_asset_framebuffer_smoke PORT=COM17 monitor
```

`upload` programuje firmware przez PlatformIO, a następnie zapisuje `assets/generated/assets.bin`
do partycji `assets` pod adresem `0x610000`. Assety są generowane przed kompilacją i nie są
wersjonowane w Git.
