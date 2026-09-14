PIO ?= platformio
PORT ?= COM23
ENV ?= cyd_display_asset_stream_api_framebuffer_smoke_psram
PIO_RUN = powershell -NoProfile -ExecutionPolicy Bypass -File "$(abspath ../../../../../tools/platformio.ps1)"
ASSET_OFFSET ?= 0x130000

.PHONY: clean assets compile upload monitor

clean:
	$(PIO_RUN) run -e $(ENV) -t clean

assets:
	python ../../../../../../brick_framework/tools/assets/generate_image_asset.py --input ../../../../esp32s3/panel_480x480/display_asset_stream_api_framebuffer_smoke/assets/joy_tears.png --symbol brick_joy_tears_320 --format rgb565 --resize 320x240 --binary assets/generated/joy_tears_320_rgb565.bin
	python ../../../../../../brick_framework/tools/assets/generate_image_asset.py --input ../../../../esp32s3/panel_480x480/display_asset_stream_api_framebuffer_smoke/assets/sweat_smile.png --symbol brick_sweat_smile_320 --format rgb565 --resize 320x240 --binary assets/generated/sweat_smile_320_rgb565.bin
	python ../../../../../../brick_framework/tools/assets/generate_solid_rgb565.py --width 320 --height 240 --color FF0000 --output assets/generated/red_background_rgb565.bin
	python ../../../../../../brick_framework/tools/assets/generate_solid_rgb565.py --width 320 --height 240 --color 0000FF --output assets/generated/blue_background_rgb565.bin
	python ../../../../../../brick_framework/tools/assets/bundle_assets.py --output assets/generated/assets.bin --header src/generated_assets.h --manifest assets/generated/assets.tsv --asset joy_tears=assets/generated/joy_tears_320_rgb565.bin,320x240,rgb565 --asset sweat_smile=assets/generated/sweat_smile_320_rgb565.bin,320x240,rgb565 --asset red_background=assets/generated/red_background_rgb565.bin,320x240,rgb565 --asset blue_background=assets/generated/blue_background_rgb565.bin,320x240,rgb565

compile: assets
	$(PIO_RUN) run -e $(ENV)

upload: assets
	$(PIO_RUN) run -e $(ENV) -t upload --upload-port $(PORT)
	python -m esptool --chip esp32 --port $(PORT) --baud 460800 write_flash $(ASSET_OFFSET) assets/generated/assets.bin

monitor:
	$(PIO) device monitor -e $(ENV) -p $(PORT) -b 115200
