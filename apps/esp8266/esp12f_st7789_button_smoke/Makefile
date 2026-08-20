PIO ?= platformio
ENV ?= esp12f_st7789_button_smoke
PORT ?= COM11
PIO_RUN = powershell -NoProfile -ExecutionPolicy Bypass -File "$(abspath ../../../tools/platformio.ps1)"

.PHONY: clean compile upload monitor

clean:
	$(PIO_RUN) run -e $(ENV) -t clean

compile:
	$(PIO_RUN) run -e $(ENV)

upload:
	$(PIO_RUN) run -e $(ENV) -t upload --upload-port $(PORT)

monitor:
	$(PIO) device monitor -p $(PORT) -b 115200
