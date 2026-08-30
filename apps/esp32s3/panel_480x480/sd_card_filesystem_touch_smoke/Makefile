PIO ?= platformio
ENV ?= esp32s3_480_sd_card_filesystem_touch_smoke
PIO_RUN = powershell -NoProfile -ExecutionPolicy Bypass -File "$(abspath ../../../../tools/platformio.ps1)"

.PHONY: clean compile upload monitor font

PYTHON ?= python
ASSET_TOOLS ?= ../../../../../brick_framework/tools/assets
FONT_SOURCE ?= ../display_asset_stream_api_framebuffer_smoke/assets/Roboto-Regular.ttf
FONT_OUTPUT ?= src/generated_font.h
FONT_LARGE_SOURCE ?= $(FONT_SOURCE)
FONT_LARGE_OUTPUT ?= src/generated_font_large.h

clean:
	$(PIO_RUN) run -e $(ENV) -t clean

compile: font
	$(PIO_RUN) run -e $(ENV)

upload: font
	$(PIO_RUN) run -e $(ENV) -t upload

monitor:
	$(PIO) device monitor -e $(ENV)

font:
	$(PYTHON) $(ASSET_TOOLS)/generate_bitmap_font.py --font $(FONT_SOURCE) --output $(FONT_OUTPUT) --name brick_roboto_20 --size 20
	$(PYTHON) $(ASSET_TOOLS)/generate_bitmap_font.py --font $(FONT_LARGE_SOURCE) --output $(FONT_LARGE_OUTPUT) --name brick_roboto_28 --size 28
