include ../../../../../tools/app.mk
ENV ?= cyd_display_color_calibration

.PHONY: clean compile upload monitor

clean:
	$(PIO) run -e $(ENV) -t clean

compile:
	$(PIO) run -e $(ENV)

upload:
	$(PIO) run -e $(ENV) -t upload --upload-port $(PORT)

monitor:
	$(PIO) device monitor -e $(ENV) -p $(PORT)
