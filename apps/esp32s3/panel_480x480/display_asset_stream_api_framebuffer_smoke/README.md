# ESP32-S3 AssetStreamer framebuffer test

Test używa wspólnego `brick::core::image::AssetStreamer` do odczytu assetu BIN
480x480 porcjami po 20 wierszy do back-framebuffer’a w PSRAM. Pełna klatka jest
prezentowana dopiero po synchronizacji VSYNC.

Assety są pakowane do jednego pliku `assets/generated/assets.bin` i zapisywane
do partycji `assets` pod adresem `0x310000`. Plik nie zawiera nagłówków pomiędzy
assetami; offsety i rozmiary generuje `bundle_assets.py` do
`src/generated_assets.h`, a plik `assets/generated/assets.tsv` służy do
diagnostyki i narzędzi uploadujących. Adapter `PartitionAssetSource` tłumaczy
offset względny assetu na adres w partycji.

```powershell
make assets
make compile
make upload
make monitor
```

`make upload` wgrywa aplikację oraz osobno `assets.bin`. Jeśli zmieni się tablica
partycji, adres można nadpisać: `make ASSET_OFFSET=0x310000 upload`.

Log raportuje czas wczytania assetu do framebuffer’a. Test nie korzysta z LVGL;
sprawdza bezpośrednio ścieżkę asset → framebuffer → page flip.
