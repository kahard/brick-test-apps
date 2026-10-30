include ../../../../tools/app.mk
ENV ?= esp32s3_480_display_asset_stream_api_framebuffer_smoke
ASSET_OFFSET ?= 0x310000

.PHONY: clean compile upload monitor assets

clean:
	$(PIO) run -e $(ENV) -t clean

compile: assets
	$(PIO) run -e $(ENV)

upload: assets
	$(PIO) run -e $(ENV) -t upload --upload-port $(PORT)
	$(PYTHON) -m esptool --chip esp32s3 --port $(PORT) --baud 460800 write_flash $(ASSET_OFFSET) assets/generated/assets.bin

monitor:
	$(PIO) device monitor -e $(ENV) -p $(PORT)

assets:
	$(PYTHON) $(ASSET_TOOLS)/generate_image_asset.py --input assets/joy_tears.png --symbol brick_joy_tears_stream --format rgb565 --resize 480x480 --binary assets/generated/joy_tears_stream_rgb565.bin
	$(PYTHON) $(ASSET_TOOLS)/generate_image_asset.py --input assets/sweat_smile.png --symbol brick_sweat_smile_stream --format rgb565 --resize 480x480 --binary assets/generated/sweat_smile_stream_rgb565.bin
	$(PYTHON) $(ASSET_TOOLS)/generate_solid_rgb565.py --width 480 --height 480 --color FF0000 --output assets/generated/red_background_rgb565.bin
	$(PYTHON) $(ASSET_TOOLS)/generate_solid_rgb565.py --width 480 --height 480 --color 0000FF --output assets/generated/blue_background_rgb565.bin
	$(PYTHON) $(ASSET_TOOLS)/bundle_assets.py --output assets/generated/assets.bin --header src/generated_assets.h --manifest assets/generated/assets.tsv --asset joy_tears=assets/generated/joy_tears_stream_rgb565.bin,480x480,rgb565 --asset sweat_smile=assets/generated/sweat_smile_stream_rgb565.bin,480x480,rgb565 --asset red_background=assets/generated/red_background_rgb565.bin,480x480,rgb565 --asset blue_background=assets/generated/blue_background_rgb565.bin,480x480,rgb565
