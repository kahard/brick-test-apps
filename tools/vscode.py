#!/usr/bin/env python3
"""Write the VS Code IntelliSense configuration for the test applications."""

from __future__ import annotations

import json
import os

import devenv

INCLUDE_PATHS = [
    "${workspaceFolder}/libs/brick/libs/interfaces/include",
    "${workspaceFolder}/libs/brick/libs/core/include",
    "${workspaceFolder}/libs/brick/platforms/esp32/include",
    "${workspaceFolder}/libs/brick/platforms/esp8266/include",
    "${workspaceFolder}/libs/brick/platforms/stm32/include",
    "${workspaceFolder}/libs/brick/boards/include",
    "${workspaceFolder}/apps/**/include",
    "${workspaceFolder}/apps/**/src",
    "${workspaceFolder}/apps/**/.pio/libdeps/**/include",
    "${workspaceFolder}/.pio/libdeps/**/include",
]


def main() -> None:
    home = "${env:USERPROFILE}" if os.name == "nt" else "${env:HOME}"
    compiler = home + "/.platformio/packages/toolchain-xtensa-esp-elf/bin/xtensa-esp32s3-elf-g++"
    if os.name == "nt":
        compiler += ".exe"
    properties = {
        "configurations": [
            {
                "name": "BRICK test apps",
                "compilerPath": compiler,
                "cStandard": "c17",
                "cppStandard": "c++17",
                "includePath": INCLUDE_PATHS,
                "defines": [],
                "browse": {
                    "path": ["${workspaceFolder}/libs/brick", "${workspaceFolder}/apps"],
                    "limitSymbolsToIncludedHeaders": True,
                },
            }
        ],
        "version": 4,
    }
    settings = {
        "cmake.ignoreCMakeListsMissing": True,
        "C_Cpp.default.configurationProvider": "platformio.platformio-ide",
        "python.defaultInterpreterPath": str(devenv.venv_python()),
    }
    vscode = devenv.REPO_ROOT / ".vscode"
    vscode.mkdir(exist_ok=True)
    (vscode / "c_cpp_properties.json").write_text(json.dumps(properties, indent=2) + "\n", encoding="utf-8")
    (vscode / "settings.json").write_text(json.dumps(settings, indent=2) + "\n", encoding="utf-8")
    print(f"VS Code IntelliSense configuration written to {vscode}")


if __name__ == "__main__":
    main()
