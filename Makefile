.PHONY: all build-all

all: build-all

build-all:
	powershell -NoProfile -ExecutionPolicy Bypass -File tools/build-all.ps1
