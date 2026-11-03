# Shared variables for application Makefiles. Include this file and use
# $(PIO) and $(PYTHON) in recipes; both run from the repository's .venv
# (create it with `make setup` in the repository root).
BRICK_REPO := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/..)
ifeq ($(OS),Windows_NT)
PYTHON := $(BRICK_REPO)/.venv/Scripts/python.exe
else
PYTHON := $(BRICK_REPO)/.venv/bin/python
endif
PIO := $(PYTHON) $(BRICK_REPO)/tools/pio.py
ASSET_TOOLS := $(BRICK_REPO)/libs/brick/tools/assets

PORT ?=
JOBS ?= 2
UPLOAD_PORT = $(if $(strip $(PORT)),--upload-port $(PORT),)
MONITOR_PORT = $(if $(strip $(PORT)),-p $(PORT),)

# PC simulator of the application (named in its pc/simulator.cmake):
#   make sim        in an SDL window (mouse = touch, space = button)
#   make sim-web    in a web browser at http://localhost:$(WEB_PORT)/
#   make sim-lan    the same, also open to other devices on the network
#                   (a phone, a tablet) at http://<this PC>:$(WEB_PORT)/
# SIM_ARGS adds simulator options, e.g. SIM_ARGS="--offline".
WEB_PORT ?= 8080
SIM_ARGS ?=
BRICK_APP_DEFAULT_GOAL := $(.DEFAULT_GOAL)

.PHONY: sim sim-web sim-lan

sim:
	$(PYTHON) $(BRICK_REPO)/tools/pc.py app $(SIM_ARGS)

sim-web:
	$(PYTHON) $(BRICK_REPO)/tools/pc.py app --web $(WEB_PORT) $(SIM_ARGS)

sim-lan:
	$(PYTHON) $(BRICK_REPO)/tools/pc.py app --web $(WEB_PORT) --web-lan $(SIM_ARGS)

# These rules do not change which target the including Makefile runs by default.
.DEFAULT_GOAL := $(BRICK_APP_DEFAULT_GOAL)
