PIO ?= platformio
ENV ?= esp32s3_480_sd_card_filesystem_touch_smoke
PIO_RUN = powershell -NoProfile -ExecutionPolicy Bypass -File "$(abspath ../../../../tools/platformio.ps1)"

.PHONY: clean compile upload monitor

clean:
	$(PIO_RUN) run -e $(ENV) -t clean

compile:
	$(PIO_RUN) run -e $(ENV)

upload:
	$(PIO_RUN) run -e $(ENV) -t upload

monitor:
	$(PIO) device monitor -e $(ENV)
