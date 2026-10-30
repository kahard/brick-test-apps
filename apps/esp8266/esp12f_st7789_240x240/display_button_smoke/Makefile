include ../../../../tools/app.mk
ENV ?= esp12f_st7789_button_smoke

.PHONY: clean compile upload monitor

clean:
	$(PIO) run -e $(ENV) -t clean

compile:
	$(PIO) run -e $(ENV)

upload:
	$(PIO) run -e $(ENV) -t upload --upload-port $(PORT)

monitor:
	$(PIO) device monitor -p $(PORT) -b 115200
