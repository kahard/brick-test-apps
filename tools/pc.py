#!/usr/bin/env python3
"""Build, test and run the PC simulator variants of the applications.

Examples:
  python tools/pc.py build                 # configure + build every simulator
  python tools/pc.py test                  # build + run the headless smoke tests
  python tools/pc.py run cyd_display_touch_smoke [simulator options...]
  python tools/pc.py list

Requires CMake and a C++17 compiler; SDL2 (optional) enables the window.
"""

from __future__ import annotations

import os
import shutil
import subprocess
import sys

import devenv

SOURCE_DIR = devenv.REPO_ROOT / "pc"
BUILD_DIR = devenv.REPO_ROOT / "build" / "pc"


def configure() -> int:
    if (BUILD_DIR / "CMakeCache.txt").exists():
        return 0
    command = ["cmake", "-S", str(SOURCE_DIR), "-B", str(BUILD_DIR), "-DCMAKE_BUILD_TYPE=Debug"]
    make = shutil.which("make")
    if make:
        # The make the other tools use; CMake's own search may find shell wrappers.
        command.append(f"-DBRICK_MAKE={make}")
    if shutil.which("ninja"):
        command += ["-G", "Ninja"]
    elif os.name == "nt" and shutil.which("mingw32-make"):
        command += ["-G", "MinGW Makefiles"]
    return subprocess.call(command)


def build() -> int:
    return configure() or subprocess.call(["cmake", "--build", str(BUILD_DIR), "--parallel"])


def executable(name: str) -> str:
    return str(BUILD_DIR / (name + (".exe" if os.name == "nt" else "")))


def main() -> int:
    if len(sys.argv) < 2 or sys.argv[1] not in ("build", "test", "run", "list"):
        print(__doc__)
        return 2
    command = sys.argv[1]
    if shutil.which("cmake") is None:
        sys.exit("error: cmake not found")
    if command == "list":
        if configure() != 0:
            return 1
        return subprocess.call(["ctest", "--test-dir", str(BUILD_DIR), "-N"])
    if build() != 0:
        return 1
    if command == "test":
        return subprocess.call(["ctest", "--test-dir", str(BUILD_DIR), "--output-on-failure"])
    if command == "run":
        if len(sys.argv) < 3:
            sys.exit("usage: python tools/pc.py run <simulator> [options...]")
        return subprocess.call([executable(sys.argv[2]), *sys.argv[3:]])
    return 0


if __name__ == "__main__":
    sys.exit(main())
