include tools/app.mk

# Python used only to create .venv; everything else runs from .venv.
ifeq ($(OS),Windows_NT)
BOOTSTRAP_PYTHON ?= py -3
else
BOOTSTRAP_PYTHON ?= python3
endif

.PHONY: all setup setup-packages doctor list build-all clean-all setup-vscode format format-check

all: build-all

setup:
	$(BOOTSTRAP_PYTHON) tools/setup.py

setup-packages:
	$(BOOTSTRAP_PYTHON) tools/setup.py --packages

doctor:
	$(BOOTSTRAP_PYTHON) tools/doctor.py

list:
	$(PYTHON) tools/apps.py list

build-all:
	$(PYTHON) tools/apps.py build $(if $(FILTER),--filter $(FILTER),) -j $(JOBS)

clean-all:
	$(PYTHON) tools/apps.py clean $(if $(FILTER),--filter $(FILTER),)

setup-vscode:
	$(PYTHON) tools/vscode.py

format:
	$(PYTHON) tools/format.py format

format-check:
	$(PYTHON) tools/format.py check

.PHONY: pc pc-test rules rules-check

# PC simulator variants of the applications (see pc/CMakeLists.txt).
pc:
	$(PYTHON) tools/pc.py build

pc-test:
	$(PYTHON) tools/pc.py test

# Embedded coding rules report (docs/embedded-coding-rules.md in the framework).
rules:
	$(PYTHON) libs/brick/libs/rules/brick_rules.py --summary apps

# The same check failing on any finding, as in CI.
rules-check:
	$(PYTHON) libs/brick/libs/rules/brick_rules.py --strict apps
