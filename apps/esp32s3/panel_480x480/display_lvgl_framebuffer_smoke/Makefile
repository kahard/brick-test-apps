ENV ?= esp32s3_480_display_lvgl_framebuffer_smoke

compile:
	platformio run -e $(ENV)

upload:
	platformio run -e $(ENV) -t upload

monitor:
	platformio device monitor -e $(ENV)
