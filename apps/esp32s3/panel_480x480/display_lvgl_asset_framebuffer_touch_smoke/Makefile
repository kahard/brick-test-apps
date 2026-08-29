ENV ?= esp32s3_480_display_lvgl_asset_framebuffer_touch_smoke

.PHONY: clean compile upload monitor assets

clean:
	platformio run -e $(ENV) -t clean
PYTHON ?= python
ASSET_TOOLS ?= ../../../../../brick_framework/tools/assets
FONT_SOURCE ?= assets/Roboto-Regular.ttf
FONT_OUTPUT ?= src/fonts/brick_roboto_24.c
IMAGE_SOURCE ?= assets/sweat_smile.png
IMAGE_OUTPUT ?= assets/generated/sweat_smile_rgb565.bin
IMAGE_BINARY_SOURCE ?= assets/joy_tears.png
IMAGE_BINARY_OUTPUT ?= assets/generated/joy_tears_rgb565.bin

compile: assets
	platformio run -e $(ENV)

upload: assets
	platformio run -e $(ENV) -t upload

monitor:
	platformio device monitor -e $(ENV)

assets:
	$(PYTHON) $(ASSET_TOOLS)/generate_lvgl_font.py --font assets/Roboto-Regular.ttf --output src/fonts/brick_roboto_24.c --name brick_roboto_24 --size 24 --bpp 4
	$(PYTHON) $(ASSET_TOOLS)/generate_image_asset.py --input assets/sweat_smile.png --symbol brick_sweat_smile --format rgb565 --resize 100x100 --binary assets/generated/sweat_smile_rgb565.bin
	$(PYTHON) $(ASSET_TOOLS)/generate_image_asset.py --input assets/joy_tears.png --symbol brick_joy_tears --format rgb565 --resize 100x100 --binary assets/generated/joy_tears_rgb565.bin
	$(PYTHON) $(ASSET_TOOLS)/generate_image_asset.py --input assets/joy_tears.png --symbol brick_background --format rgb565 --resize 480x480 --binary assets/generated/background_rgb565.bin
	$(PYTHON) $(ASSET_TOOLS)/bundle_assets.py --output assets/generated/assets.bin --header src/generated_assets.h --c-source src/asset_bundle.cpp --c-symbol brick_asset_bundle --asset sweat_smile=assets/generated/sweat_smile_rgb565.bin,100x100,rgb565 --asset joy_tears=assets/generated/joy_tears_rgb565.bin,100x100,rgb565 --asset full_background=assets/generated/background_rgb565.bin,480x480,rgb565

font:
	$(PYTHON) $(ASSET_TOOLS)/generate_lvgl_font.py --font $(FONT_SOURCE) --output $(FONT_OUTPUT) --name brick_roboto_24 --size 24 --bpp 4

image:
	$(PYTHON) $(ASSET_TOOLS)/generate_image_asset.py --input $(IMAGE_SOURCE) --symbol brick_sweat_smile --format rgb565 --resize 100x100 --binary $(IMAGE_OUTPUT)

image-bin:
	$(PYTHON) $(ASSET_TOOLS)/generate_image_asset.py --input $(IMAGE_BINARY_SOURCE) --symbol brick_joy_tears --format rgb565 --resize 100x100 --binary $(IMAGE_BINARY_OUTPUT)

assets: font image image-bin
