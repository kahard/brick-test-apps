PIO ?= platformio
ENV ?= jc1060_7_display_framebuffer_smoke
PORT ?=
JOBS ?= 2
PYTHON ?= python
PIO_RUN = powershell -NoProfile -ExecutionPolicy Bypass -File "$(abspath ../../../../tools/platformio.ps1)"
UPLOAD_PORT = $(if $(strip $(PORT)),--upload-port $(PORT),)
MONITOR_PORT = $(if $(strip $(PORT)),-p $(PORT),)

.PHONY: clean compile upload monitor assets
clean:
	$(PIO_RUN) run -e $(ENV) -t clean
compile:
	$(PIO_RUN) run -e $(ENV) -j $(JOBS)
upload:
	$(PIO_RUN) run -e $(ENV) -j $(JOBS) -t upload $(UPLOAD_PORT)
monitor:
	$(PIO) device monitor -e $(ENV) $(MONITOR_PORT)
