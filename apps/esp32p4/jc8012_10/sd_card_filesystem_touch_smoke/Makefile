include ../../../../tools/app.mk
ENV ?= jc8012_10_sd_card_filesystem_touch_smoke

.PHONY: clean compile upload monitor

FONT_SOURCE ?= ../../../esp32s3/panel_480x480/display_asset_stream_api_framebuffer_smoke/assets/Roboto-Regular.ttf
FONT_OUTPUT ?= src/generated/generated_font.h
FONT_LARGE_OUTPUT ?= src/generated/generated_font_large.h
clean:
	$(PIO) run -e $(ENV) -t clean
compile: font
	$(PIO) run -e $(ENV)
upload: font
	$(PIO) run -e $(ENV) -t upload $(if $(PORT),--upload-port $(PORT),)
monitor:
	$(PIO) device monitor -e $(ENV) $(if $(PORT),-p $(PORT),)

font:
	$(PYTHON) -c "from pathlib import Path; Path('src/generated').mkdir(parents=True, exist_ok=True)"
	$(PYTHON) $(ASSET_TOOLS)/generate_bitmap_font.py --font $(FONT_SOURCE) --output $(FONT_OUTPUT) --name brick_roboto_20 --size 20
	$(PYTHON) $(ASSET_TOOLS)/generate_bitmap_font.py --font $(FONT_SOURCE) --output $(FONT_LARGE_OUTPUT) --name brick_roboto_28 --size 28
