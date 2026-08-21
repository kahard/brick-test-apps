PIO_RUN = powershell -NoProfile -ExecutionPolicy Bypass -File "$(abspath ../../../../../tools/platformio.ps1)"
ENV ?= cyd_psram_lvgl_framebuffer_smoke
PORT ?= COM17
PYTHON ?= python
ASSET_TOOLS ?= ../../../../../../brick_framework/tools/assets
.PHONY: clean assets font images compile upload monitor
clean:
	$(PIO_RUN) run -e $(ENV) -t clean
font:
	$(PYTHON) $(ASSET_TOOLS)/generate_lvgl_font.py --font ../../../../esp32s3/panel_480x480/display_lvgl_asset_smoke/assets/Roboto-Regular.ttf --output src/fonts/brick_roboto_20.c --name brick_roboto_20 --size 20 --bpp 4
images:
	$(PYTHON) $(ASSET_TOOLS)/generate_image_asset.py --input ../../../../esp32s3/panel_480x480/display_lvgl_asset_smoke/assets/joy_tears.png --symbol brick_joy_tears --format rgb565 --resize 100x100 --binary assets/generated/joy_tears_rgb565.bin
	$(PYTHON) $(ASSET_TOOLS)/generate_image_asset.py --input ../../../../esp32s3/panel_480x480/display_lvgl_asset_smoke/assets/sweat_smile.png --symbol brick_sweat_smile --format rgb565 --resize 100x100 --binary assets/generated/sweat_smile_rgb565.bin
	$(PYTHON) $(ASSET_TOOLS)/bundle_assets.py --output assets/generated/assets.bin --header src/generated_assets.h --manifest assets/generated/assets.tsv --asset joy_tears=assets/generated/joy_tears_rgb565.bin,100x100,rgb565 --asset sweat_smile=assets/generated/sweat_smile_rgb565.bin,100x100,rgb565
assets: font images
compile: assets
	$(PIO_RUN) run -e $(ENV)
upload: assets
	$(PIO_RUN) run -e $(ENV) -t upload --upload-port $(PORT)
	python -m esptool --chip esp32 --port $(PORT) --baud 460800 write_flash 0x170000 assets/generated/assets.bin
monitor:
	platformio device monitor -e $(ENV) -p $(PORT) -b 115200
