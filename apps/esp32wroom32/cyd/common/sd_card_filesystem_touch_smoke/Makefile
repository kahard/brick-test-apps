include ../../../../../tools/app.mk
ENV ?= cyd_sd_card_filesystem_touch_smoke
FONT_SOURCE ?= ../../../../esp32s3/panel_480x480/display_asset_stream_api_framebuffer_smoke/assets/Roboto-Regular.ttf

.PHONY: clean compile upload monitor font

clean:
	$(PIO) run -e $(ENV) -t clean

compile: font
	$(PIO) run -e $(ENV)

upload: font
	$(PIO) run -e $(ENV) -t upload --upload-port $(PORT)

monitor:
	$(PIO) device monitor -e $(ENV) -p $(PORT)

font:
	$(PYTHON) -c "from pathlib import Path; Path('src/generated').mkdir(parents=True, exist_ok=True)"
	$(PYTHON) $(ASSET_TOOLS)/generate_bitmap_font.py --font $(FONT_SOURCE) --output src/generated/generated_font.h --name brick_roboto_16 --size 16
	$(PYTHON) $(ASSET_TOOLS)/generate_bitmap_font.py --font $(FONT_SOURCE) --output src/generated/generated_font_large.h --name brick_roboto_22 --size 22
