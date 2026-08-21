ENV ?= esp32s3_480_display_lvgl_framebuffer_touch_smoke

.PHONY: clean compile upload monitor

clean:
	platformio run -e $(ENV) -t clean

compile:
	platformio run -e $(ENV)

upload:
	platformio run -e $(ENV) -t upload

monitor:
	platformio device monitor -e $(ENV)
