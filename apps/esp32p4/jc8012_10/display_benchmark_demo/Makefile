include ../../../../tools/app.mk
ENV ?= jc8012_10_display_benchmark_demo

.PHONY: clean compile upload monitor
clean:
	$(PIO) run -e $(ENV) -t clean
compile:
	$(PIO) run -e $(ENV) -j $(JOBS)
upload:
	$(PIO) run -e $(ENV) -j $(JOBS) -t upload $(UPLOAD_PORT)
monitor:
	$(PIO) device monitor -e $(ENV) $(MONITOR_PORT)
