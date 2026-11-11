include ../../../../tools/app.mk
ENV ?= esp32s3_480_display_benchmark_demo

.PHONY: clean compile upload monitor

clean:
	$(PIO) run -e $(ENV) -t clean

compile:
	$(PIO) run -e $(ENV)

upload:
	$(PIO) run -e $(ENV) -t upload

monitor:
	$(PIO) device monitor -e $(ENV)
