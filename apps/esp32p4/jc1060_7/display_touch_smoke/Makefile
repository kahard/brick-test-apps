include ../../../../tools/app.mk
ENV ?= jc1060_7_display_touch_smoke

.PHONY: clean compile upload monitor

clean:
	$(PIO) run -e $(ENV) -t clean

compile:
	$(PIO) run -e $(ENV) -j $(JOBS)

upload:
	$(PIO) run -e $(ENV) -j $(JOBS) -t upload $(if $(PORT),--upload-port $(PORT),)

monitor:
	$(PIO) device monitor -e $(ENV) $(if $(PORT),-p $(PORT),)
