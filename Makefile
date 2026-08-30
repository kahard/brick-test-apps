.PHONY: all clean-all build-all setup-vscode format format-check

all: build-all

clean-all:
	powershell -NoProfile -ExecutionPolicy Bypass -File tools/clean-all.ps1

build-all:
	powershell -NoProfile -ExecutionPolicy Bypass -File tools/build-all.ps1

setup-vscode:
	powershell -NoProfile -ExecutionPolicy Bypass -File tools/setup-vscode.ps1

format:
	bash ./tools/clang.sh format

format-check:
	bash ./tools/clang.sh format-check
