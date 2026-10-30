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
