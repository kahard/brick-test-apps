include ../../../../tools/app.mk
ENV ?= esp12f_st7789_spi_benchmark_40mhz

.PHONY: clean assets compile compile-all upload monitor

assets:
	$(PYTHON) "$(ASSET_TOOLS)/generate_image_asset.py" --input "../../../esp32s3/panel_480x480/display_lvgl_asset_smoke/assets/joy_tears.png" --output src/app/assets/joy_tears_240_rgb565.c --symbol brick_joy_tears_240 --format rgb565 --resize 240x240 --progmem
	$(PYTHON) "$(ASSET_TOOLS)/generate_image_asset.py" --input "../../../esp32s3/panel_480x480/display_lvgl_asset_smoke/assets/sweat_smile.png" --output src/app/assets/sweat_smile_240_rgb565.c --symbol brick_sweat_smile_240 --format rgb565 --resize 240x240 --progmem

clean:
	$(PIO) run -e $(ENV) -t clean

compile:
	$(PIO) run -e $(ENV)

compile-all:
	$(PIO) run -e esp12f_st7789_spi_benchmark_40mhz -e esp12f_st7789_spi_benchmark_60mhz -e esp12f_st7789_spi_benchmark_80mhz

upload:
	$(PIO) run -e $(ENV) -t upload --upload-port $(PORT)

monitor:
	$(PIO) device monitor -e $(ENV) -p $(PORT) -b 115200
