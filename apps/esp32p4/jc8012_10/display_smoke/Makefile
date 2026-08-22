PIO ?= platformio
ENV ?= jc8012_10_display_smoke_r0
PIO_RUN = powershell -NoProfile -ExecutionPolicy Bypass -File "$(abspath ../../../../tools/platformio.ps1)"

.PHONY: clean compile compile-0 compile-90 compile-180 compile-270 upload upload-0 upload-90 upload-180 upload-270 monitor
clean:
	$(PIO_RUN) run -e $(ENV) -t clean
compile:
	$(PIO_RUN) run -e $(ENV)
compile-0:
	$(PIO_RUN) run -e jc8012_10_display_smoke_r0
compile-90:
	$(PIO_RUN) run -e jc8012_10_display_smoke_r90
compile-180:
	$(PIO_RUN) run -e jc8012_10_display_smoke_r180
compile-270:
	$(PIO_RUN) run -e jc8012_10_display_smoke_r270
upload:
	$(PIO_RUN) run -e $(ENV) -t upload
upload-0:
	$(PIO_RUN) run -e jc8012_10_display_smoke_r0 -t upload
upload-90:
	$(PIO_RUN) run -e jc8012_10_display_smoke_r90 -t upload
upload-180:
	$(PIO_RUN) run -e jc8012_10_display_smoke_r180 -t upload
upload-270:
	$(PIO_RUN) run -e jc8012_10_display_smoke_r270 -t upload
monitor:
	$(PIO) device monitor -e $(ENV)
