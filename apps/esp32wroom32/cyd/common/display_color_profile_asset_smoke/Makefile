PIO ?= platformio
PORT ?= COM20
ENV ?= cyd_display_color_profile_asset_smoke
PIO_RUN = powershell -NoProfile -ExecutionPolicy Bypass -File "$(abspath ../../../../../tools/platformio.ps1)"

.PHONY: clean assets compile upload monitor

clean:
	$(PIO_RUN) run -e $(ENV) -t clean

assets:
	python ../../../../../../brick_framework/tools/assets/generate_image_asset.py --input assets/source/ebu_colorchecker_portrait.jpg --symbol brick_ebu_colorchecker_portrait --format rgb565 --resize 320x240 --binary assets/generated/ebu_colorchecker_portrait_rgb565.bin
	python ../../../../../../brick_framework/tools/assets/bundle_assets.py --output assets/generated/assets.bin --header src/generated_assets.h --manifest assets/generated/assets.tsv --asset ebu_colorchecker_portrait=assets/generated/ebu_colorchecker_portrait_rgb565.bin,320x240,rgb565

compile: assets
	$(PIO_RUN) run -e $(ENV)

upload: assets
	$(PIO_RUN) run -e $(ENV) -t upload --upload-port $(PORT)
	python -m esptool --chip esp32 --port $(PORT) --baud 460800 write_flash 0x170000 assets/generated/assets.bin

monitor:
	$(PIO) device monitor -e $(ENV) -p $(PORT)
