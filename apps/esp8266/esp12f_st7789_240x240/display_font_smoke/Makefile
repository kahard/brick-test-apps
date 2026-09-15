PIO ?= platformio
ENV ?= esp12f_st7789_display_font_smoke
PORT ?= COM11
PIO_RUN = powershell -NoProfile -ExecutionPolicy Bypass -File "$(abspath ../../../../tools/platformio.ps1)"
.PHONY: fonts clean compile upload monitor
fonts:
	python "../../../../../brick_framework/tools/assets/generate_bitmap_font.py" --font "../../../esp32s3/panel_480x480/display_asset_stream_api_framebuffer_smoke/assets/Roboto-Regular.ttf" --output src/generated/font_12.h --name brick_roboto_12 --size 12
	python "../../../../../brick_framework/tools/assets/generate_bitmap_font.py" --font "../../../esp32s3/panel_480x480/display_asset_stream_api_framebuffer_smoke/assets/Roboto-Regular.ttf" --output src/generated/font_20.h --name brick_roboto_20 --size 20
	python "../../../../../brick_framework/tools/assets/generate_bitmap_font.py" --font "../../../esp32s3/panel_480x480/display_asset_stream_api_framebuffer_smoke/assets/Roboto-Regular.ttf" --output src/generated/font_28.h --name brick_roboto_28 --size 28
clean:
	$(PIO_RUN) run -e $(ENV) -t clean
compile: fonts
	$(PIO_RUN) run -e $(ENV)
upload:
	$(PIO_RUN) run -e $(ENV) -t upload --upload-port $(PORT)
monitor:
	$(PIO) device monitor -e $(ENV) -p $(PORT) -b 115200
