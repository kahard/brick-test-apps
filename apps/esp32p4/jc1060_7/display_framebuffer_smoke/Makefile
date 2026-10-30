include ../../../../tools/app.mk
ENV ?= jc1060_7_display_framebuffer_smoke

.PHONY: clean compile upload monitor assets
clean:
	$(PIO) run -e $(ENV) -t clean
compile:
	$(PIO) run -e $(ENV) -j $(JOBS)
upload:
	$(PIO) run -e $(ENV) -j $(JOBS) -t upload $(UPLOAD_PORT)
monitor:
	$(PIO) device monitor -e $(ENV) $(MONITOR_PORT)
