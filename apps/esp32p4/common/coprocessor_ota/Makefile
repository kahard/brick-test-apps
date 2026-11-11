include ../../../../tools/app.mk
ENV ?= esp32p4_coprocessor_ota
# The co-processor firmware built by build_coprocessor_firmware.ps1.
IMAGE ?= .pio/coprocessor/network_adapter.bin

.PHONY: clean compile upload image monitor

clean:
	$(PIO) run -e $(ENV) -t clean

compile:
	$(PIO) run -e $(ENV) -j $(JOBS)

upload:
	$(PIO) run -e $(ENV) -j $(JOBS) -t upload $(UPLOAD_PORT)

# Writes the co-processor image into the coprocessor_fw partition (it is an
# ESP32-C6 image, hence --force) without restarting the P4.
image:
	$(PYTHON) -m esptool --chip esp32p4 $(if $(strip $(PORT)),--port $(PORT),) --baud 460800 --after no_reset write_flash --force 0x210000 $(IMAGE)

monitor:
	$(PIO) device monitor -e $(ENV) $(MONITOR_PORT)
