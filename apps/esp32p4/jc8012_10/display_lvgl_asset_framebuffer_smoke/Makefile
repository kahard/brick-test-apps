PIO ?= platformio
ENV ?= jc8012_10_display_lvgl_asset_framebuffer_smoke_r0
PORT ?= COM11
PYTHON ?= python
ASSET_TOOLS ?= ../../../../../brick_framework/tools/assets
PIO_RUN = powershell -NoProfile -ExecutionPolicy Bypass -File "$(abspath ../../../../tools/platformio.ps1)"

.PHONY: clean assets images compile compile-0 compile-90 compile-180 compile-270 compile-all compile-touch-0 compile-touch-90 compile-touch-180 compile-touch-270 compile-touch-all compile-fps-color compile-fps-asset upload upload-0 upload-90 upload-180 upload-270 upload-touch-0 upload-touch-90 upload-touch-180 upload-touch-270 upload-fps-color upload-fps-asset monitor
clean:
	$(PIO_RUN) run -e $(ENV) -t clean
images:
	$(PYTHON) $(ASSET_TOOLS)/generate_image_asset.py --input ../../../esp32s3/panel_480x480/display_lvgl_asset_smoke/assets/joy_tears.png --symbol brick_joy_tears --format rgb565 --resize 100x100 --binary assets/generated/joy_tears_rgb565.bin
	$(PYTHON) $(ASSET_TOOLS)/generate_image_asset.py --input ../../../esp32s3/panel_480x480/display_lvgl_asset_smoke/assets/sweat_smile.png --symbol brick_sweat_smile --format rgb565 --resize 100x100 --binary assets/generated/sweat_smile_rgb565.bin
	$(PYTHON) $(ASSET_TOOLS)/bundle_assets.py --output assets/generated/assets.bin --header src/generated_assets.h --manifest assets/generated/assets.tsv --asset joy_tears=assets/generated/joy_tears_rgb565.bin,100x100,rgb565 --asset sweat_smile=assets/generated/sweat_smile_rgb565.bin,100x100,rgb565
assets: images
compile: compile-all
compile-0: assets
	$(PIO_RUN) run -e jc8012_10_display_lvgl_asset_framebuffer_smoke_r0
compile-90: assets
	$(PIO_RUN) run -e jc8012_10_display_lvgl_asset_framebuffer_smoke_r90
compile-180: assets
	$(PIO_RUN) run -e jc8012_10_display_lvgl_asset_framebuffer_smoke_r180
compile-270: assets
	$(PIO_RUN) run -e jc8012_10_display_lvgl_asset_framebuffer_smoke_r270
compile-all: compile-0 compile-90 compile-180 compile-270 compile-touch-all compile-fps-color compile-fps-asset
compile-touch-0: assets
	$(PIO_RUN) run -e jc8012_10_display_lvgl_asset_framebuffer_smoke_touch_r0
compile-touch-90: assets
	$(PIO_RUN) run -e jc8012_10_display_lvgl_asset_framebuffer_smoke_touch_r90
compile-touch-180: assets
	$(PIO_RUN) run -e jc8012_10_display_lvgl_asset_framebuffer_smoke_touch_r180
compile-touch-270: assets
	$(PIO_RUN) run -e jc8012_10_display_lvgl_asset_framebuffer_smoke_touch_r270
compile-touch-all: compile-touch-0 compile-touch-90 compile-touch-180 compile-touch-270
compile-fps-color: assets
	$(PIO_RUN) run -e jc8012_10_display_lvgl_asset_framebuffer_smoke_fps_color_r0
compile-fps-asset: assets
	$(PIO_RUN) run -e jc8012_10_display_lvgl_asset_framebuffer_smoke_fps_asset_r0
upload: assets
	$(PIO_RUN) run -e $(ENV) -t upload --upload-port $(PORT)
	python -m esptool --chip esp32p4 --port $(PORT) --baud 460800 write_flash 0x610000 assets/generated/assets.bin
upload-0: assets
	$(PIO_RUN) run -e jc8012_10_display_lvgl_asset_framebuffer_smoke_r0 -t upload --upload-port $(PORT)
	python -m esptool --chip esp32p4 --port $(PORT) --baud 460800 write_flash 0x610000 assets/generated/assets.bin
upload-90: assets
	$(PIO_RUN) run -e jc8012_10_display_lvgl_asset_framebuffer_smoke_r90 -t upload --upload-port $(PORT)
	python -m esptool --chip esp32p4 --port $(PORT) --baud 460800 write_flash 0x610000 assets/generated/assets.bin
upload-180: assets
	$(PIO_RUN) run -e jc8012_10_display_lvgl_asset_framebuffer_smoke_r180 -t upload --upload-port $(PORT)
	python -m esptool --chip esp32p4 --port $(PORT) --baud 460800 write_flash 0x610000 assets/generated/assets.bin
upload-270: assets
	$(PIO_RUN) run -e jc8012_10_display_lvgl_asset_framebuffer_smoke_r270 -t upload --upload-port $(PORT)
	python -m esptool --chip esp32p4 --port $(PORT) --baud 460800 write_flash 0x610000 assets/generated/assets.bin
upload-touch-0: assets
	$(PIO_RUN) run -e jc8012_10_display_lvgl_asset_framebuffer_smoke_touch_r0 -t upload --upload-port $(PORT)
	python -m esptool --chip esp32p4 --port $(PORT) --baud 460800 write_flash 0x610000 assets/generated/assets.bin
upload-touch-90: assets
	$(PIO_RUN) run -e jc8012_10_display_lvgl_asset_framebuffer_smoke_touch_r90 -t upload --upload-port $(PORT)
	python -m esptool --chip esp32p4 --port $(PORT) --baud 460800 write_flash 0x610000 assets/generated/assets.bin
upload-touch-180: assets
	$(PIO_RUN) run -e jc8012_10_display_lvgl_asset_framebuffer_smoke_touch_r180 -t upload --upload-port $(PORT)
	python -m esptool --chip esp32p4 --port $(PORT) --baud 460800 write_flash 0x610000 assets/generated/assets.bin
upload-touch-270: assets
	$(PIO_RUN) run -e jc8012_10_display_lvgl_asset_framebuffer_smoke_touch_r270 -t upload --upload-port $(PORT)
	python -m esptool --chip esp32p4 --port $(PORT) --baud 460800 write_flash 0x610000 assets/generated/assets.bin
upload-fps-color: assets
	$(PIO_RUN) run -e jc8012_10_display_lvgl_asset_framebuffer_smoke_fps_color_r0 -t upload --upload-port $(PORT)
	python -m esptool --chip esp32p4 --port $(PORT) --baud 460800 write_flash 0x610000 assets/generated/assets.bin
upload-fps-asset: assets
	$(PIO_RUN) run -e jc8012_10_display_lvgl_asset_framebuffer_smoke_fps_asset_r0 -t upload --upload-port $(PORT)
	python -m esptool --chip esp32p4 --port $(PORT) --baud 460800 write_flash 0x610000 assets/generated/assets.bin
monitor:
	$(PIO) device monitor -e $(ENV) -p $(PORT) -b 115200
