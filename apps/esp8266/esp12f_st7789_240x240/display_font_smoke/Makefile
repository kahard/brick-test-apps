include ../../../../tools/app.mk
ENV ?= esp12f_st7789_display_font_smoke
.PHONY: fonts clean compile upload monitor
fonts:
	$(PYTHON) "$(ASSET_TOOLS)/generate_bitmap_font.py" --font "../../../esp32s3/panel_480x480/display_asset_stream_api_framebuffer_smoke/assets/Roboto-Regular.ttf" --output src/app/generated/font_12.h --name brick_roboto_12 --size 12
	$(PYTHON) "$(ASSET_TOOLS)/generate_bitmap_font.py" --font "../../../esp32s3/panel_480x480/display_asset_stream_api_framebuffer_smoke/assets/Roboto-Regular.ttf" --output src/app/generated/font_20.h --name brick_roboto_20 --size 20
	$(PYTHON) "$(ASSET_TOOLS)/generate_bitmap_font.py" --font "../../../esp32s3/panel_480x480/display_asset_stream_api_framebuffer_smoke/assets/Roboto-Regular.ttf" --output src/app/generated/font_28.h --name brick_roboto_28 --size 28
clean:
	$(PIO) run -e $(ENV) -t clean
compile: fonts
	$(PIO) run -e $(ENV)
upload:
	$(PIO) run -e $(ENV) -t upload --upload-port $(PORT)
monitor:
	$(PIO) device monitor -e $(ENV) -p $(PORT) -b 115200
