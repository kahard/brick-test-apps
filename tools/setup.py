#!/usr/bin/env python3
"""Prepare the development environment.

Creates .venv, installs the pinned tools from requirements.txt, checks out
missing git submodules and, with --packages, downloads the PlatformIO platforms
and toolchains used by the applications.

Any Python 3 can start it: when the running interpreter is not supported
(ESP-IDF accepts Python 3.10-3.14 and no MSYS2/Git Bash Python), the script
looks for a supported one and restarts itself with it.
"""

from __future__ import annotations

import argparse
import os
import shutil
import subprocess
import sys
import sysconfig
import venv

import devenv


def supported(version: tuple[int, int]) -> bool:
    return devenv.MIN_PYTHON <= version <= devenv.MAX_PYTHON


def native_python() -> bool:
    return not sysconfig.get_platform().startswith("mingw")


def candidates() -> list[list[str]]:
    minors = range(devenv.MAX_PYTHON[1], devenv.MIN_PYTHON[1] - 1, -1)
    if os.name == "nt" and shutil.which("py"):
        return [["py", f"-3.{minor}"] for minor in minors]
    return [[path] for minor in minors if (path := shutil.which(f"python3.{minor}"))]


def ensure_supported_python() -> None:
    if supported(sys.version_info[:2]) and native_python():
        return
    for command in candidates():
        probe = subprocess.run([*command, "-c", "import sysconfig; print(sysconfig.get_platform())"],
                               capture_output=True, text=True)
        if probe.returncode == 0 and not probe.stdout.startswith("mingw"):
            print(f"Python {sys.version.split()[0]} is not supported here; using `{' '.join(command)}`", flush=True)
            sys.exit(subprocess.call([*command, __file__, *sys.argv[1:]]))
    low, high = (".".join(map(str, v)) for v in (devenv.MIN_PYTHON, devenv.MAX_PYTHON))
    sys.exit(f"error: a native Python {low}-{high} is required (found {sys.version.split()[0]})")


def venv_version() -> tuple[int, int] | None:
    probe = subprocess.run([str(devenv.venv_python()), "-c", "import sys; print(*sys.version_info[:2])"],
                           capture_output=True, text=True)
    return tuple(map(int, probe.stdout.split())) if probe.returncode == 0 else None


def create_venv() -> None:
    if devenv.venv_python().exists():
        if venv_version() == sys.version_info[:2]:
            return
        print(f"Recreating {devenv.VENV_DIR} with Python {sys.version.split()[0]}")
        shutil.rmtree(devenv.VENV_DIR)
    else:
        print(f"Creating {devenv.VENV_DIR}")
    venv.EnvBuilder(with_pip=True).create(devenv.VENV_DIR)


def install_requirements() -> None:
    python = devenv.venv_python()
    pip = [python, "-m", "pip", "install", "--disable-pip-version-check", "-q"]
    if devenv.run([*pip, "--upgrade", "pip"]) != 0 or devenv.run([*pip, "-r", devenv.REQUIREMENTS]) != 0:
        sys.exit("error: installing requirements failed")


def init_submodules() -> None:
    """Check out missing submodules; leave already checked-out ones alone."""
    git = ["git", "-C", str(devenv.REPO_ROOT)]
    status = subprocess.run([*git, "submodule", "status"], capture_output=True, text=True).stdout
    missing = [line.split()[1] for line in status.splitlines() if line.startswith("-")]
    if missing:
        print("Initialising git submodules: " + ", ".join(missing))
        subprocess.call([*git, "submodule", "update", "--init", "--recursive", "--", *missing])


def install_packages() -> int:
    platformio = devenv.require_venv_tool("platformio")
    failed = 0
    for project in devenv.find_projects():
        print(f"Installing PlatformIO packages for {project.relative_to(devenv.REPO_ROOT)}")
        if devenv.run([platformio, "pkg", "install", "-d", project]) != 0:
            failed += 1
    return failed


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--packages", action="store_true", help="also download PlatformIO platforms and toolchains")
    parser.add_argument("--no-submodules", action="store_true", help="do not touch git submodules")
    args = parser.parse_args()

    ensure_supported_python()
    create_venv()
    install_requirements()
    if not args.no_submodules:
        init_submodules()
    if args.packages and install_packages() != 0:
        return 1
    print("Environment ready. Check it with `make doctor`.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
