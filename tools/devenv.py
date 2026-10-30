"""Shared helpers for the repository tools.

Every tool runs PlatformIO and the Python helpers from the repository's own
virtual environment (.venv) so that builds do not depend on what happens to
be installed on the host.
"""

from __future__ import annotations

import os
import subprocess
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[1]
VENV_DIR = REPO_ROOT / ".venv"
APPS_DIR = REPO_ROOT / "apps"
REQUIREMENTS = REPO_ROOT / "requirements.txt"

# Python versions accepted by the pinned ESP-IDF/pioarduino platform.
MIN_PYTHON = (3, 10)
MAX_PYTHON = (3, 14)

# ESP-IDF refuses to run from Git Bash/MSYS2 and PlatformIO may then install
# only tool metadata. Drop these variables before starting any build tool.
MSYS_VARIABLES = (
    "MSYSTEM",
    "MSYSTEM_CARCH",
    "MSYSTEM_CHOST",
    "MSYSTEM_PREFIX",
    "MINGW_CHOST",
    "MINGW_PACKAGE_PREFIX",
    "MINGW_PREFIX",
    "SHELL",
    "EXEPATH",
    "PYTHONHOME",
    "PYTHONPATH",
)

SKIPPED_DIRS = {".git", ".pio", "managed_components", "generated", "node_modules"}


def venv_bin() -> Path:
    return VENV_DIR / ("Scripts" if os.name == "nt" else "bin")


def venv_tool(name: str) -> Path:
    return venv_bin() / (name + ".exe" if os.name == "nt" else name)


def venv_python() -> Path:
    return venv_tool("python")


def clean_env() -> dict[str, str]:
    # The virtual environment is deliberately not put on PATH: its esptool.py
    # script would shadow the esptool package ESP-IDF runs during a build.
    # PlatformIO already runs extra scripts with the .venv Python.
    return {key: value for key, value in os.environ.items() if key not in MSYS_VARIABLES}


def require_venv_tool(name: str) -> Path:
    tool = venv_tool(name)
    if not tool.exists():
        sys.exit(f"error: {tool} not found; run `make setup` (or `python tools/setup.py`) first")
    return tool


def run(command: list[str | os.PathLike[str]], cwd: Path | None = None) -> int:
    return subprocess.call([str(part) for part in command], cwd=cwd, env=clean_env())


def find_projects(root: Path = APPS_DIR) -> list[Path]:
    """Directories under apps/ that contain a platformio.ini."""
    projects = []
    for directory, subdirs, files in os.walk(root):
        subdirs[:] = sorted(d for d in subdirs if d not in SKIPPED_DIRS)
        if "platformio.ini" in files:
            projects.append(Path(directory))
    return projects
