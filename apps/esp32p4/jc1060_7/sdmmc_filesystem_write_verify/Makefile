PIO ?= platformio
ENV ?= jc1060_7_sdmmc_filesystem_write_verify

.PHONY: clean compile upload monitor
clean:
	$(PIO) run -e $(ENV) -t clean
compile:
	$(PIO) run -e $(ENV)
upload:
	$(PIO) run -e $(ENV) -t upload
monitor:
	$(PIO) device monitor -e $(ENV)

