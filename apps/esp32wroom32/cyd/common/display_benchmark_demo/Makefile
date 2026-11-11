include ../../../../../tools/app.mk
ENV ?= cyd_display_benchmark_demo

.PHONY: clean compile upload monitor

clean:
	$(PIO) run -e $(ENV) -t clean

compile:
	$(PIO) run -e $(ENV)

upload:
	$(PIO) run -e $(ENV) -t upload $(UPLOAD_PORT)

monitor:
	$(PIO) device monitor -e $(ENV) $(MONITOR_PORT)
