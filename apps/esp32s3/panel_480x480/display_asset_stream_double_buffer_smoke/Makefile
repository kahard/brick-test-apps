PIO ?= platformio
ENV ?= esp32s3_480_display_asset_stream_double_buffer_smoke
PIO_RUN = powershell -NoProfile -ExecutionPolicy Bypass -File "$(abspath ../../../../tools/platformio.ps1)"

.PHONY: clean compile upload monitor assets

clean:
	$(PIO_RUN) run -e $(ENV) -t clean

compile: assets
	$(PIO_RUN) run -e $(ENV)

upload: assets
	$(PIO_RUN) run -e $(ENV) -t upload

monitor:
	$(PIO) device monitor -e $(ENV)

assets:
	python ../../../../../brick_framework/tools/assets/generate_image_asset.py --input assets/joy_tears.png --symbol brick_joy_tears_stream --format rgb565 --resize 480x480 --binary assets/generated/joy_tears_stream_rgb565.bin
	python ../../../../../brick_framework/tools/assets/generate_image_asset.py --input assets/sweat_smile.png --symbol brick_sweat_smile_stream --format rgb565 --resize 480x480 --binary assets/generated/sweat_smile_stream_rgb565.bin
	python ../../../../../brick_framework/tools/assets/bundle_assets.py --output assets/generated/assets.bin --header src/generated_assets.h --c-source src/asset_bundle.cpp --c-symbol brick_asset_bundle --asset joy_tears=assets/generated/joy_tears_stream_rgb565.bin,480x480,rgb565 --asset sweat_smile=assets/generated/sweat_smile_stream_rgb565.bin,480x480,rgb565
