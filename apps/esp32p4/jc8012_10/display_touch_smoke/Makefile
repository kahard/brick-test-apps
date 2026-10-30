include ../../../../tools/app.mk
ENV ?= jc8012_10_display_touch_smoke_r0

.PHONY: clean clean-all clean-0 clean-90 clean-180 clean-270 compile compile-all compile-0 compile-90 compile-180 compile-270 upload upload-0 upload-90 upload-180 upload-270 monitor

clean: clean-all

clean-all: clean-0 clean-90 clean-180 clean-270

clean-0:
	$(PIO) run -e jc8012_10_display_touch_smoke_r0 -t clean

clean-90:
	$(PIO) run -e jc8012_10_display_touch_smoke_r90 -t clean

clean-180:
	$(PIO) run -e jc8012_10_display_touch_smoke_r180 -t clean

clean-270:
	$(PIO) run -e jc8012_10_display_touch_smoke_r270 -t clean

compile: compile-all

compile-all: compile-0 compile-90 compile-180 compile-270

compile-0:
	$(PIO) run -e jc8012_10_display_touch_smoke_r0 -j $(JOBS)

compile-90:
	$(PIO) run -e jc8012_10_display_touch_smoke_r90 -j $(JOBS)

compile-180:
	$(PIO) run -e jc8012_10_display_touch_smoke_r180 -j $(JOBS)

compile-270:
	$(PIO) run -e jc8012_10_display_touch_smoke_r270 -j $(JOBS)

upload:
	$(PIO) run -e $(ENV) -j $(JOBS) -t upload $(if $(PORT),--upload-port $(PORT),)

upload-0:
	$(PIO) run -e jc8012_10_display_touch_smoke_r0 -j $(JOBS) -t upload $(if $(PORT),--upload-port $(PORT),)

upload-90:
	$(PIO) run -e jc8012_10_display_touch_smoke_r90 -j $(JOBS) -t upload $(if $(PORT),--upload-port $(PORT),)

upload-180:
	$(PIO) run -e jc8012_10_display_touch_smoke_r180 -j $(JOBS) -t upload $(if $(PORT),--upload-port $(PORT),)

upload-270:
	$(PIO) run -e jc8012_10_display_touch_smoke_r270 -j $(JOBS) -t upload $(if $(PORT),--upload-port $(PORT),)

monitor:
	$(PIO) device monitor -e $(ENV) $(if $(PORT),-p $(PORT),)
