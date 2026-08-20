PIO ?= platformio
ENV ?= esp32s3_480_display_asset_stream_api_framebuffer_smoke
PIO_RUN = powershell -NoProfile -ExecutionPolicy Bypass -File "$(abspath ../../../../tools/platformio.ps1)"

.PHONY: clean compile upload monitor assets

clean:
	$(PIO_RUN) run -e $(ENV) -t clean

compile:
	$(PIO_RUN) run -e $(ENV)

upload:
	$(PIO_RUN) run -e $(ENV) -t upload

monitor:
	$(PIO) device monitor -e $(ENV)

assets:
	python ../../../../../brick_framework/tools/assets/generate_image_asset.py --input assets/joy_tears.png --symbol brick_joy_tears_stream --format rgb565 --resize 480x480 --binary assets/generated/joy_tears_stream_rgb565.bin
	python ../../../../../brick_framework/tools/assets/generate_image_asset.py --input assets/sweat_smile.png --symbol brick_sweat_smile_stream --format rgb565 --resize 480x480 --binary assets/generated/sweat_smile_stream_rgb565.bin
	python ../../../../../brick_framework/tools/assets/generate_solid_rgb565.py --width 480 --height 480 --color FF0000 --output assets/generated/red_background_rgb565.bin
	python ../../../../../brick_framework/tools/assets/generate_solid_rgb565.py --width 480 --height 480 --color 0000FF --output assets/generated/blue_background_rgb565.bin
