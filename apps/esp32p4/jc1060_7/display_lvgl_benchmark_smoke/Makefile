PIO ?= platformio
ENV ?= jc1060_7_display_lvgl_benchmark_smoke
PORT ?= COM11
PYTHON ?= python
ASSET_TOOLS ?= ../../../../../brick_framework/tools/assets
PIO_RUN = powershell -NoProfile -ExecutionPolicy Bypass -File "$(abspath ../../../../tools/platformio.ps1)"
.PHONY: clean assets compile upload monitor
clean:
	$(PIO_RUN) run -e $(ENV) -t clean
assets:
	$(PYTHON) $(ASSET_TOOLS)/generate_image_asset.py --input ../../../esp32s3/panel_480x480/display_lvgl_asset_smoke/assets/joy_tears.png --symbol brick_joy_tears --format rgb565 --resize 100x100 --binary assets/generated/joy_tears_rgb565.bin
	$(PYTHON) $(ASSET_TOOLS)/generate_image_asset.py --input ../../../esp32s3/panel_480x480/display_lvgl_asset_smoke/assets/sweat_smile.png --symbol brick_sweat_smile --format rgb565 --resize 100x100 --binary assets/generated/sweat_smile_rgb565.bin
	$(PYTHON) $(ASSET_TOOLS)/bundle_assets.py --output assets/generated/assets.bin --header src/generated_assets.h --manifest assets/generated/assets.tsv --asset joy_tears=assets/generated/joy_tears_rgb565.bin,100x100,rgb565 --asset sweat_smile=assets/generated/sweat_smile_rgb565.bin,100x100,rgb565
compile: assets
	$(PIO_RUN) run -e $(ENV)
upload: assets
	$(PIO_RUN) run -e $(ENV) -t upload --upload-port $(PORT)
	python -m esptool --chip esp32p4 --port $(PORT) --baud 460800 write_flash 0x610000 assets/generated/assets.bin
monitor:
	$(PIO) device monitor -e $(ENV) -p $(PORT) -b 115200
