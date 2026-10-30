include ../../../../../tools/app.mk
ENV ?= cyd_display_color_calibration

.PHONY: clean compile upload monitor pc-serial-input

# Serial input of the PC simulator smoke test (pc/simulator.cmake).
pc-serial-input:
	$(PYTHON) pc/serial_input.py pc/generated/serial_in.bin

clean:
	$(PIO) run -e $(ENV) -t clean

compile:
	$(PIO) run -e $(ENV)

upload:
	$(PIO) run -e $(ENV) -t upload --upload-port $(PORT)

monitor:
	$(PIO) device monitor -e $(ENV) -p $(PORT)
