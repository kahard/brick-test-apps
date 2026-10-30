include ../../../../../tools/app.mk
ENV ?= cyd_display_color_profile_asset_smoke

.PHONY: clean assets compile upload monitor

clean:
	$(PIO) run -e $(ENV) -t clean

assets:
	$(PYTHON) $(ASSET_TOOLS)/generate_image_asset.py --input assets/source/ebu_colorchecker_portrait.jpg --symbol brick_ebu_colorchecker_portrait --format rgb565 --resize 320x240 --binary assets/generated/ebu_colorchecker_portrait_rgb565.bin
	$(PYTHON) $(ASSET_TOOLS)/bundle_assets.py --output assets/generated/assets.bin --header src/generated_assets.h --manifest assets/generated/assets.tsv --asset ebu_colorchecker_portrait=assets/generated/ebu_colorchecker_portrait_rgb565.bin,320x240,rgb565

compile: assets
	$(PIO) run -e $(ENV)

upload: assets
	$(PIO) run -e $(ENV) -t upload --upload-port $(PORT)
	$(PYTHON) -m esptool --chip esp32 --port $(PORT) --baud 460800 write_flash 0x170000 assets/generated/assets.bin

monitor:
	$(PIO) device monitor -e $(ENV) -p $(PORT)
