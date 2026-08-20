PIO ?= platformio
ENV ?= esp12f_st7789_asset_stream_smoke
PORT ?= COM11
PIO_RUN = powershell -NoProfile -ExecutionPolicy Bypass -File "$(abspath ../../../tools/platformio.ps1)"

.PHONY: clean assets compile upload monitor

assets:
	python "../../../../brick_framework/tools/assets/generate_image_asset.py" --input "../../esp32s3/panel_480x480/display_lvgl_asset_smoke/assets/joy_tears.png" --output src/assets/joy_tears_240_rgb565.c --symbol brick_joy_tears_240 --format rgb565 --resize 240x240 --progmem
	python "../../../../brick_framework/tools/assets/generate_image_asset.py" --input "../../esp32s3/panel_480x480/display_lvgl_asset_smoke/assets/sweat_smile.png" --output src/assets/sweat_smile_240_rgb565.c --symbol brick_sweat_smile_240 --format rgb565 --resize 240x240 --progmem

clean:
	$(PIO_RUN) run -e $(ENV) -t clean

compile:
	$(PIO_RUN) run -e $(ENV)

upload:
	$(PIO_RUN) run -e $(ENV) -t upload --upload-port $(PORT)

monitor:
	$(PIO) device monitor -e $(ENV) -p $(PORT) -b 115200
