PIO ?= platformio
ENV ?= jc1060_7_display_touch_smoke
PORT ?=
JOBS ?= 2
PIO_RUN = powershell -NoProfile -ExecutionPolicy Bypass -File "$(abspath ../../../../tools/platformio.ps1)"

.PHONY: clean compile upload monitor

clean:
	$(PIO_RUN) run -e $(ENV) -t clean

compile:
	$(PIO_RUN) run -e $(ENV) -j $(JOBS)

upload:
	$(PIO_RUN) run -e $(ENV) -j $(JOBS) -t upload $(if $(PORT),--upload-port $(PORT),)

monitor:
	$(PIO) device monitor -e $(ENV) $(if $(PORT),-p $(PORT),)
