#!/usr/bin/env python3
"""Check that the development environment is complete."""

from __future__ import annotations

import shutil
import subprocess
import sys

import devenv


def output(command) -> str | None:
    try:
        result = subprocess.run([str(c) for c in command], capture_output=True, text=True, env=devenv.clean_env())
    except OSError:
        return None
    return (result.stdout or result.stderr).strip().splitlines()[0] if result.returncode == 0 else None


def pinned_versions() -> dict[str, str]:
    pins = {}
    for line in devenv.REQUIREMENTS.read_text(encoding="utf-8").splitlines():
        line = line.split("#", 1)[0].strip()
        if "==" in line:
            name, version = line.split("==", 1)
            pins[name.strip().lower()] = version.strip()
    return pins


def main() -> int:
    pins = pinned_versions()
    checks: list[tuple[str, bool, str, bool]] = []  # name, ok, detail, required

    python = devenv.venv_python()
    version = output([python, "-c", "import sys; print(*sys.version_info[:2])"]) if python.exists() else None
    if version is None:
        checks.append(("virtualenv (.venv)", False, "missing: run `make setup`", True))
    else:
        ok = devenv.MIN_PYTHON <= tuple(map(int, version.split())) <= devenv.MAX_PYTHON
        detail = "Python " + version.replace(" ", ".") + ("" if ok else ": unsupported, run `make setup`")
        checks.append(("virtualenv (.venv)", ok, detail, True))

    for package, tool, required in (("platformio", "platformio", True), ("clang-format", "clang-format", False)):
        found = output([devenv.venv_tool(tool), "--version"]) if devenv.venv_tool(tool).exists() else None
        pin = pins.get(package)
        ok = found is not None and (pin is None or pin in found)
        detail = found or "missing"
        if found and pin and pin not in found:
            detail += f" (expected {pin}: run `make setup`)"
        checks.append((package, ok, detail, required))

    framework = devenv.REPO_ROOT / "libs" / "brick" / "CMakeLists.txt"
    checks.append(("libs/brick submodule", framework.exists(), "initialised" if framework.exists() else
                   "missing: git submodule update --init --recursive", True))
    checks.append(("git", shutil.which("git") is not None, shutil.which("git") or "missing", True))
    checks.append(("make", shutil.which("make") is not None,
                   shutil.which("make") or "missing: apps that generate fonts/assets need it", True))
    npx = shutil.which("npx")
    checks.append(("npx (LVGL fonts only)", npx is not None, npx or "missing: install Node.js to regenerate LVGL fonts", False))

    failed = False
    for name, ok, detail, required in checks:
        status = "OK  " if ok else ("FAIL" if required else "WARN")
        failed |= required and not ok
        print(f"[{status}] {name:24} {detail}")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
