#!/usr/bin/env python3
"""Run the repository's pinned PlatformIO with a clean environment.

Usage: python tools/pio.py <platformio arguments...>
"""

import sys

import devenv

if __name__ == "__main__":
    sys.exit(devenv.run([devenv.require_venv_tool("platformio"), *sys.argv[1:]]))
