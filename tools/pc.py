#!/usr/bin/env python3
"""Build, test and run the PC simulator variants of the applications.

Examples:
  python tools/pc.py build                 # configure + build every simulator
  python tools/pc.py test                  # build + run the headless smoke tests
  python tools/pc.py run cyd_display_touch_smoke [simulator options...]
  python tools/pc.py app [simulator options...]   # the application in the current directory
  python tools/pc.py list

`app` builds only the simulator of the application in the current directory
(the name in its pc/simulator.cmake) and runs it; the application Makefiles
use it for `make sim` (SDL window), `make sim-web` (web browser) and
`make sim-lan` (web browser, also from other devices on the network).

Requires CMake and a C++17 compiler; SDL2 (optional) enables the window.
"""

from __future__ import annotations

import os
import re
import shutil
import socket
import subprocess
import sys
from pathlib import Path

import devenv

SOURCE_DIR = devenv.REPO_ROOT / "pc"
BUILD_DIR = devenv.REPO_ROOT / "build" / "pc"
SIMULATOR_NAME = re.compile(r"brick_app_simulator\(\s*([A-Za-z0-9_]+)")


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


def build(target: str | None = None) -> int:
    command = ["cmake", "--build", str(BUILD_DIR), "--parallel"]
    if target:
        command += ["--target", target]
    return configure() or subprocess.call(command)


def executable(name: str) -> str:
    return str(BUILD_DIR / (name + (".exe" if os.name == "nt" else "")))


def simulator_of(app_dir: Path) -> str:
    """The simulator target an application declares in pc/simulator.cmake."""
    description = app_dir / "pc" / "simulator.cmake"
    if not description.is_file():
        sys.exit(f"error: {app_dir} has no pc/simulator.cmake")
    match = SIMULATOR_NAME.search(description.read_text(encoding="utf-8"))
    if match is None:
        sys.exit(f"error: no brick_app_simulator() in {description}")
    return match.group(1)


def lan_addresses() -> list[str]:
    """IPv4 addresses of this computer, the one of the default route first."""
    addresses: list[str] = []
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as probe:
            probe.connect(("192.0.2.1", 9))  # sends nothing; picks the default route
            addresses.append(probe.getsockname()[0])
    except OSError:
        pass
    try:
        for info in socket.getaddrinfo(socket.gethostname(), None, socket.AF_INET):
            address = info[4][0]
            if not address.startswith(("127.", "169.254.")) and address not in addresses:
                addresses.append(address)
    except OSError:
        pass
    return addresses


def print_web_urls(options: list[str]) -> None:
    """Where to open the web screen: one URL per line, set apart by blank lines,
    so a terminal turns each into a link. With --web-lan also the addresses
    other devices can use, which the simulator itself cannot know."""
    port = "8080"
    following = options[options.index("--web") + 1 :]
    if following and following[0].isdigit():
        port = following[0]
    print()
    print(f"  Web screen:      http://localhost:{port}/")
    if "--web-lan" in options:
        label = "  On the network:  "
        for address in lan_addresses():
            print(f"{label}http://{address}:{port}/")
            label = " " * len(label)
        print("  (if another device cannot connect, allow the simulator in the firewall)")
    print()
    sys.stdout.flush()


def run(name: str, options: list[str]) -> int:
    if "--web" in options:
        print_web_urls(options)
    try:
        return subprocess.call([executable(name), *options])
    except KeyboardInterrupt:  # Ctrl+C ends an interactive run
        return 0


def main() -> int:
    if len(sys.argv) < 2 or sys.argv[1] not in ("build", "test", "run", "app", "list"):
        print(__doc__)
        return 2
    command = sys.argv[1]
    if shutil.which("cmake") is None:
        sys.exit("error: cmake not found")
    if command == "list":
        if configure() != 0:
            return 1
        return subprocess.call(["ctest", "--test-dir", str(BUILD_DIR), "-N"])
    if command == "app":
        name = simulator_of(Path.cwd())
        return build(name) or run(name, sys.argv[2:])
    if build() != 0:
        return 1
    if command == "test":
        return subprocess.call(["ctest", "--test-dir", str(BUILD_DIR), "--output-on-failure"])
    if command == "run":
        if len(sys.argv) < 3:
            sys.exit("usage: python tools/pc.py run <simulator> [options...]")
        return run(sys.argv[2], sys.argv[3:])
    return 0


if __name__ == "__main__":
    sys.exit(main())
