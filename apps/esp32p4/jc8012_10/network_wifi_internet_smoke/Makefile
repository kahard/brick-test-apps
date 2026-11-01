include ../../../../tools/app.mk
ENV ?= jc8012_10_network_wifi_internet_smoke

.PHONY: clean compile upload monitor font

FONT_SOURCE ?= ../../../esp32s3/panel_480x480/display_asset_stream_api_framebuffer_smoke/assets/Roboto-Regular.ttf
FONT_OUTPUT ?= src/app/generated/network_font.h
FONT_SIZE ?= 20

clean:
	$(PIO) run -e $(ENV) -t clean

compile: font
	$(PIO) run -e $(ENV)

upload: font
	$(PIO) run -e $(ENV) -t upload $(UPLOAD_PORT)

monitor:
	$(PIO) device monitor -e $(ENV) $(MONITOR_PORT)

font:
	$(PYTHON) -c "from pathlib import Path; Path('src/app/generated').mkdir(parents=True, exist_ok=True)"
	$(PYTHON) $(ASSET_TOOLS)/generate_bitmap_font.py --font $(FONT_SOURCE) --output $(FONT_OUTPUT) --name brick_network_font --size $(FONT_SIZE)
